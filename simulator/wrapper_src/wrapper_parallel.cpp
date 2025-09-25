#include <boost/format.hpp>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include "parse.hpp"
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <omp.h>
#include <sched.h>    
    
std::vector<int> get_available_cpus() {    
    // Create a CPU set and initialize it    
    cpu_set_t cpu_set;       
    CPU_ZERO(&cpu_set);      
                             
    // Get the affinity mask for the current process (PID 0 means current process)    
    if (sched_getaffinity(0, sizeof(cpu_set), &cpu_set) == -1) {    
        perror("sched_getaffinity");    
        return {};           
    }                        
     
    // Collect the available CPUs    
    std::vector<int> available_cpus;    
    for (int i = 0; i < CPU_SETSIZE; ++i) {    
        if (CPU_ISSET(i, &cpu_set)) {    
            available_cpus.push_back(i);    
        }    
    }    
     
    return available_cpus;    
}

class LocalData {
public:
  double threshold; /* maximal stderr requested (if positive) */
  double relstderr; /* observed stderr so far */
  double mean; /* mean of benched times, to be used if the block is disabled */
  double sum;  /* sum of benched times (to compute the mean and stderr) */
  double sum_pow2; /* sum of the square of the benched times (to compute the
                      stderr) */
  int iters;       /* amount of requested iterations */
  int count;       /* amount of iterations done so far */
  bool benching;   /* true: we are benchmarking; false: we have enough data, no
                      bench anymore */

  bool need_more_benchs() const;
};

bool LocalData::need_more_benchs() const {
  bool res =
      (count < iters) && (threshold < 0.0 || count < 2 || // not enough data
                          relstderr >= threshold);        // stderr too high yet
  // fprintf(stderr, "\r%s (count:%d sum: %f iter:%d stderr:%f thres:%f mean:%fs)",
  //          (res ? "need more data" : "enough benchs"), count, sum, iters,
  //          relstderr, threshold, mean);
  return res;
}

int main(int argc, char **argv) {
  if (argc < 8) {
    std::cerr << "Usage: " << argv[0]
              << " <platform_file> <hostfile> <executable> <benchmark> <thresholds> "
                 "<max_iters> <byte_sizes>"
              << std::endl;
    return 1;
  }


  const std::string platform_file = argv[1];
  const std::string hostfile = argv[2];
  const std::string executable = argv[3];
  std::string benchmark = argv[4];       // grab from cmdline
  int max_iters = std::stoi(argv[6]);    // grab from cmdline

  std::string byte_string = argv[7];
  std::vector<std::string> byte_sizes;

  std::string threshold_string = argv[5];
  std::vector<std::string> thresholds;
  boost::split(thresholds, threshold_string, boost::is_any_of(","));
  boost::split(byte_sizes, byte_string, boost::is_any_of(","));

  std::vector<std::string> final_benchmarks(24, "");
  
  std::vector<int> cpus = get_available_cpus();

  fprintf(stderr, "Available CPUs: %d\n", (int) cpus.size());

  // Setting num_procs to run
  omp_set_num_threads(byte_sizes.size());


  FILE *original_stdout = fdopen(dup(fileno(stdout)), "w");

  #pragma omp parallel 
  {
    int rank = omp_get_thread_num();

    std::string filename = "p2p_" + std::to_string(rank) + ".log";

    std::string byte = byte_sizes[rank];

    LocalData data = LocalData{
        std::stod(thresholds[rank]), // threshold
        0.0,       // relstderr
        0.0,       // mean
        0.0,       // sum
        0.0,       // sum_pow2
        max_iters, // iters
        0,         // count
        true       // benching (if we have no data, we need at least one)
    };
    
    std::string command = "";
    std::string iterations = "1";

    if (std::stoi(byte) <= 8192) {
      iterations = "10";
    }

    // command = "taskset -c " + std::to_string(cpus[rank % cpus.size()]) + " ";

    command += "smpirun -platform " + platform_file + " -hostfile " + hostfile + " " + executable + " " + benchmark+ " -iter " + iterations + " -msgsz " + byte ;

    for (int j = 8; j < argc; j++) {
      command += " ";
      command += argv[j];
    }

    command += " > ";
    command += filename;

    #pragma omp critical
    {
      std::cerr << "---------------" << std::endl;

      std::cerr << "[" << rank << "] Benchmarking with " << byte << " byte" << std::endl;
    
      std::cerr << command << std::endl;
      
      std::cerr << "---------------" << std::endl;
    }
  
  
    for (int k = 0; k < max_iters; k++) {
      std::system(command.c_str());
    
      std::vector<BenchmarkData> benchmarkMap = parse_file(filename);

      std::string rm_command = "rm " + filename;

    
      if (benchmarkMap.size() != 1) {
  std::cerr << "Rank [" << rank << "]: assertion failed! BenchmarkMap's size: " << benchmarkMap.size() << std::endl;
  abort();
      }

      std::system(rm_command.c_str());

      // update the stats
      data.count++;
      double mb_per_sec = benchmarkMap[0].mb_per_sec;
      data.sum         += mb_per_sec;
      data.sum_pow2    += mb_per_sec * mb_per_sec;
      double n          = data.count;
      data.mean         = data.sum / n;
      data.relstderr    = std::sqrt((data.sum_pow2 / n) - (data.mean * data.mean)) / data.mean;

      fprintf(stderr, "[%d] Iteration %d: %.2f relstderr %.2f MBps\n", rank, k, data.relstderr, mb_per_sec);
      if (!data.need_more_benchs()) {
        final_benchmarks[rank] = boost::str(boost::format("%.2f") % data.mean);
        fprintf(stderr, "[%d] Iterations: %d\n", rank, data.count);
        break;
      }
    }
  }


  std::string result = boost::algorithm::join(final_benchmarks, " ");

  stdout = original_stdout;

  fprintf(stdout, "%s\n", result.c_str());
  fprintf(stderr, "Result: %s\n\n", result.c_str());

  return 0;
}


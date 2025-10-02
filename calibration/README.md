
---
# Overview

This directory contains scripts and utilities for calibrating and simulating MPI performance, as well as ground truth generation and supporting configuration files.

## Python Scripts
- `run_smpi_calibrator.py`: Main entry point to run the SMPI calibration workflow.

- `SMPISimulator.py`: Defines the `SMPISimulator` class, which implements the core SMPI simulation. Can be ran on its own to run a single run of the simulator.

- `SMPISimulatorCalibrator.py`: Defines the `SMPISimulatorCalibrator` class, which utilizes Simcal to perform and manage the calibration process for SMPI simulations.

- `calibrate_flops.py`: Performs FLOPS (floating point operations per second) calibration for the simulation environment. Used to estimate computational performance.

- `mpi_groundtruth.py`: Parses ground-truth data to be used for the calibration. See [figshare](https://doi.org/10.6084/m9.figshare.30132955) for ground-truth data used for the experiments.

- `Utils.py`: Provides utility functions shared across scripts, such as different loss functions.

## Default configuration files

- `defaults/hostfile.txt`: Example hostfile listing nodes or hosts used in calibration or simulation.

- `defaults/params.txt`: Default parameter values for calibration and simulation runs.

The following section provides detailed descriptions and usage instructions for the runnable scripts in this directory.

---

# Running the Scripts

## `SMPISimulator.py`
This file not only implements the `SMPISimulator` class but also functions as a command-line utility that takes a list of simulator arguments to execute a single simulation run, invoked as:

```bash
./SMPISimulator.py
```

## `run_smpi_calibrator.py`
This script is a command-line utility used to calibrate the simulator. The script will create an output file named `result.json` that contains the configuration used for the calibration under the property `config`, and the results of the calibration (which contains the best values for each simulation parameter, loss value, and the result of the simulation) under the property `results`.

```bash
./run_smpi_calibrator.py
    -gf <path_to_ground_truth_file>  # Required
    [byte_sizes]
    [-top <path_to_topology.json>]
    [-sc]
    [-s <comma_separated_splits>]
    [-lf {max,average}]
    [-la {max_agg,average_agg}]
    [-hf <path_to_hostfile>]
    [-b <comma_separated_benchmarks>]
    [-n <comma_separated_node_counts>]
    [-a {grid, random, gradient, skopt.gp, skopt.et, skopt.rf, skopt.gbrt}]
    [-t <time_limit>]
    [-p <path_to_param_file>]
    [-d]
    [--verbose]
```

### Required Arguments

* `--ground_truth_file`, `-gf`
    * **Description**: Specifies the path to the ground truth file. See [figshare](https://doi.org/10.6084/m9.figshare.30132955) for ground-truth data used for the experiments.
    * **Type**: `string`
    * **Required**: Yes

### Positional Arguments

* `byte_sizes`
    * **Description**: A positional argument for a comma-separated list of byte sizes to use during calibration.
    * **Type**: `list[int]`
    * **Default**: `1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304`
    * **Example**: `1024,2048,4096`

### Optional Arguments

* `--topology`, `-top`
    * **Description**: Path to the JSON configuration file that defines the cluster topology.
    * **Type**: `string`
    * **Default**: `config/6-racks-no-gpu-no-nvme.json`

* `--simple_compute`, `-sc`
    * **Description**: A boolean flag that, when present, instructs the simulator to use simple compute nodes.
    * **Type**: `boolean` (flag)
    * **Default**: `False`

* `--split`, `-s`
    * **Description**: A comma-separated list of integer splits to use for the latency/bandwidth factor.
    * **Type**: `list[int]`
    * **Default**: `None`
    * **Example**: `--split 1024,16384,524288`
> [!NOTE]
>  Defining the argument requires you to add a corresponding `network/latency-factor_{i}` or `network/bandwidth-factor_{i}` to `params.txt`, where `i` refers to the i<sup>th</sup> integer split. See `calibration/defaults/params.txt` for more information.

* `--loss_function`, `-lf`
    * **Description**: Sets the explained variance loss function to use.
    * **Type**: `string`
    * **Choices**: `max`, `average`
    * **Default**: `average`

* `--loss_aggregator`, `-la`
    * **Description**: Sets the explained variance loss aggregator to use.
    * **Type**: `string`
    * **Choices**: `max_agg`, `average_agg`
    * **Default**: `average_agg`

* `--hostfile`, `-hf`
    * **Description**: Specifies the path to the hostfile.
    * **Type**: `string`
    * **Default**: `defaults/hostfile.txt`

* `--benchmarks`, `-b`
    * **Description**: A comma-separated list of benchmark names to use for calibration.
    * **Type**: `list[string]`
    * **Default**: `Birandom,PingPing,PingPong`
    * **Example**: `--benchmarks PingPing,PingPong,Corandom,Unirandom`

* `--node_counts`, `-n`
    * **Description**: A comma-separated list of node counts to use for calibration.
    * **Type**: `list[int]`
    * **Default**: A pre-defined list of node counts.
    * **Example**: `--node_counts 128,256,512`
>[!NOTE]
> Ensure that the ground-truth include data of the specified node count.

* `--algorithm`, `-a`
    * **Description**: Defines the algorithm to be used for calibration.
    * **Type**: `string`
    * **Choices**: `grid`, `random`, `gradient`, `skopt.gp`, `skopt.et`, `skopt.rf`, `skopt.gbrt`
    * **Default**: `random`

* `--time_limit`, `-t`
    * **Description**: Sets the time limit for the calibration process.
    * **Type**: `string`
    * **Default**: `3h`
    * **Example** `--time_limit 2d`, `--time-limit 10m`
>[!NOTE]
> See [pytimeparse](https://github.com/wroberts/pytimeparse) for all available time expressions.

* `--param_file`, `-p`
    * **Description**: Specifies the path to the parameter file.
    * **Type**: `string`
    * **Default**: `defaults/params.txt`

The following are utility arguments and has no effect on the calibration process.

* `--debug`, `-d`
    * **Description**: A boolean flag that enables debug-level logging.
    * **Type**: `boolean` (flag)
    * **Default**: `False`

* `--verbose`
    * **Description**: A boolean flag that enables verbose output mode.
    * **Type**: `boolean` (flag)
    * **Default**: `False`

---


---
## About

This repository contains the software used for Case Study #2 in [https://doi.org/10.1145/3731599.3767698](https://doi.org/10.1145/3731599.3767698), with the objective of making the experiments and results in that case study reproducible.

The software is composed of two parts:

  - **The Simulator**: A simulation of Intel MPI benchmark implemented with SimGrid, located in the `simulator/` directory.
  - **Calibration scripts**: A set of python scripts that utilize Simcal to calibrate the simulator, located in the `calibration/` directory.


The experiments in the case study are conducted with the ground-truth data available on [figshare](https://doi.org/10.6084/m9.figshare.30132955).

---

## Environment Setup

A `Dockerfile` is provided in the root directory to build a Docker image that has all the necessary software installed.

In particular the Docker image will include:
- [SimGrid 4.0](https://framagit.org/simgrid/simgrid/)
- [WRENCH 2.6](https://github.com/wrench-project/wrench)
- [Simcal](https://github.com/wrench-project/simcal) (commit tag `86445d59177922fa3711473bbf4e5e207005fcc2` was used in the experiments).  
- The simulator in `simulator/`

### Using Docker

Make sure you are in the root directory of this repository before running the following commands.

1. **Build the Docker image**:

    ```
    docker build -t case-study-2-env .
    ```

2. **Run the container**:

    This command will launch an interactive bash shell inside the container. The current directory on your host machine will be mounted into the `/workspace` directory inside the container.

    ```
    docker run -it -v $(pwd):/workspace case-study-2-env bash
    ```

    Once inside the container's shell, you can proceed to the "Running the Software" section.

### Manual Installation

Reading the `Dockerfile` should make it very clear how to install all necessary software locally on a recen
t Ubuntu system. 

---

## Running the Software

### MPI Execution Simulator

The simulator is invoked by the calibration scripts in (see next section). But the simulator can also be invoked stand-alone. 

Please refer to the `README.md` in the `simulator/` directory for detailed guideline on how to run the simulator.

### Calibration Scripts

Please refer to the `README.md` in the `calibration/` directory for detailed guidelines on how to run the calibration scripts.

---





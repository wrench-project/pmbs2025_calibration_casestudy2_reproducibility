
---
# About

This repository contains the software used for Case Study #2 in https://doi.org/10.1145/3731599.3767698, with the objective of making the experiments and results in that case study reproducible.

The software is composed of two main parts:

  - **The Simulator**: A simulation of Intel MPI benchmark using SimGrid, located in the `simulator/` directory.
  - **Calibration scripts**: A set of python scripts that utlizing simcal to calibrate the simulator, located in the `calibration/` directory.


The experiments in the case study are conducted with the ground-truth data available on [figshare](https://doi.org/10.6084/m9.figshare.30132955).

---

# Environment Setup
We have provided a Dockerfile in the root directory with all the necessary software installed.

In particular the Docker image will include:
- [SimGrid 4.0](https://framagit.org/simgrid/simgrid/)
- [WRENCH 2.6](https://github.com/wrench-project/wrench)
- [Simcal](https://github.com/wrench-project/simcal) (commit tag `86445d59177922fa3711473bbf4e5e207005fcc2` was used in the experiments).  
- The simulator in `simulator/`

## Using the Dockerfile

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

    Once inside the container's shell, you can proceed to the "Running the Experiments" section.

## Manual Installation

*This section is a work in progress. For now, please use the recommended Docker setup above to ensure a consistent environment.*


# Running the Experiments





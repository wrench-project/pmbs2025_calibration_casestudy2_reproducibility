
---
# Overview
This directory contains scripts and utilities for calibrating and simulating MPI performance, as well as ground truth generation and supporting configuration files.

## Python Scripts
- `run_smpi_calibrator.py`: Main entry point to run the SMPI calibration workflow.

- `SMPISimulator.py`: Defines the `SMPISimulator` class, which implements the core SMPI simulation. Can be ran on its own to run a single run of the simulator.

- `SMPISimulatorCalibrator.py`: Defines the `SMPISimulatorCalibrator` class, which utilizes Simcal to o perform and manage the calibration process for SMPI simulations.

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
This file not only implements the SMPISimulator class but also functions as a command-line utility that takes a list of simulator arguments to execute a single simulation run.

```bash
./SMPISimulator.py
```
## `run_smpi_calibrator.py`

```bash
./run_smpi_calibrator.py
```
---
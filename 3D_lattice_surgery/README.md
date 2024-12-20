# Materials for lattice surgery simulations for local Hamiltonians

## Requirement

- OS: Windows, Linux, or MacOS
- C++ compiler: any compiler with C++14 standard (verified with GCC >= 9 or Visual Studio >= 2019)
- Python >= 3.7
  - numpy
  - matplotlib
  - openfermion
  - qutip
- CMake >= 3

## Overview

- circuit_synth: Generating and translating circuit relevant to Qubitization
- emulator: Map logical qubit to plane and route a sequence of lattice surgery instructions

## Usage

### 1. Generate Circuit

Move to `circuit_synth` and generate circuits with `generate_circuit.py`

```
cd circuit_synth
python generate_circuit.py

```
If library is not installed, please install with `pip`.

The generated circuits are saved to `./circuit_synth/circuit/`.
The filename represents the following.
`result_<circuit_name>_<hamiltonian_size>_<hamiltonian_name>_{boundary_condition}_{param1}_{param2}_{parallelize_level}.raw`



### 2. Compile logical qubit mapper and lattice surgery router

Move to `emulator` and build executables

In the case of Linux, MacOS, or WSL on Windows, please use the following commands.

```
cd ../emulator
mkdir build
cd build
cmake ..
make

```

execute.
```

make run_all

```

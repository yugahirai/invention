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
mkdir ./build
cd build
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release ..
cd ../
cmake --build ./build --target all --config Release
```

In the case of Windows, plase use the following commands.
```
cd ../emulator
mkdir visualstudio
cd visualstudio
cmake ..
cd ../
cmake --build ./visualstudio --target ALL_BUILD --config Release
```

If the above command finishes, there are three executables `ftqc_placement`, `ftqc_routing`, and `ftqc_routing_visualize`.

### 3. Map logical qubit allocations.

We can allocate the localtion of logical qubit to plane with `ftqc_placement`. The usage of this file is 
`./bin/ftqc_placement <instruction_file> <output_placement_file> <number_of_magic_state_factories> <type_of_hamiltonian>`
For example, the following codes will work for example.
```
./bin/ftqc_placement ../circuit_synth/circuit/result_SELECT_9_Heisenberg2D_cylinder_0.5_0.5_5.raw placement.txt 28 Heisenberg2D
```

If it is properly executed, the following messages will be found.
```
instruction: ../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_2.raw
result_name: placement.txt
num_factory: 4
hamiltonian_type: Heisenberg2D
finish
```

### 4. Debug routing with visualization
We can check the routing and placement with `ftqc_routing_visualize`. The usage of this file is 
`./bin/ftqc_routing_visualize <instruction_file> <placement_file> <output_routing_file> <number_of_magic_state_factories> <size_of_instruction_queue>`

The instruction queue is a maxmimum number of instruction distance that become the target of simultaneous execution.
For example, the following codes will work for example.
```
./bin/ftqc_routing_visualize ../circuit_synth/circuit/result_SELECT_9_Heisenberg2D_cylinder_0.5_0.5_5.raw placement.txt result.txt 28 100
```

Animation will progress in each key push.

```
--- CODE BEAT:53  INST:179/9064 ---
--- FLOOR STR ---
M..........................
M..QQ.QQ.QQ.QQ.Q..QQ.QQ.QQ.
M**...............QQ.QQ.QQ.
M.*QQ.QQ.QQ.QQ.Q...........
M.*...............QQ.QQ.QQ.
M.*QQ.QQ.QQ.QQ.Q..QQ.QQ.QQ.
M.*........................
M.*QQ*QQ.QQ.QQ.Q..QQ.QQ.QQ.
M.*...............QQ.QQ.QQ.
M.*QQ.QQ.QQ.QQ.Q...........
..*.**.....................
..*QQ*QQ.QQ.QQ.Q...........
..****.....................
...QQ*QQ.QQ.QQ.Q...........
.....*.....................
...QQ*QQ.QQ.QQ.Q...........
...........................

--- INST queue ---
178 : SurgeryMagic control_ancilla_1_th_2 out creg391 lock:1 magic
179 : PhaseTwist control_ancilla_1_th_2 cond creg391
180 : Surgery control_ancilla_0_th_2 control_ancilla_1_th_2 lock:2
181 : SurgeryMagic control_ancilla_1_th_2 out creg392 lock:1 magic
182 : PhaseTwist control_ancilla_1_th_2 cond creg392
183 : Hadamard control_ancilla_1_th_2
184 : PhaseTwist control_ancilla_1_th_2
186 : Surgery control_ancilla_0_th_3 control_ancilla_1_th_3 lock:2
187 : SurgeryMagic control_ancilla_1_th_3 out creg393 lock:1 magic
```

Here, `FLOOR_STR` represents current status of logical-qubit floor. 
```
M: magic state factory
Q: persistent logical qubit
*: temporal logical qubit for lattice surgery or code deformation
```

`INST queue` represents the list of instruction currently in the queue.
The first integer is the index of the instruction. The instruction mnemonic and operands. 


### 5. Generate routing file

```
./bin/ftqc_routing ../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_2.raw placement.txt result.txt 10 10
```

Then, `result.txt` will be generated.
      

### 6. Plot the results

With the following commands, we can stat the execution sequence.
```
python plot.py result.txt
```

## Brief explanation of instruction list

```
ALLOCATE: Alloacte logical qubit
MAGIC: Allocate magic state factory
DEST_MEAS: Destructive measurement with X or Z basis
MEAS_TWIST: Twist Y measurement
PHASE_TWIST: Twist S gate
HADAMARD: Logical Hadamard gate
SURGERY: Perform two-body lattice surgery
MAGIC_SURGERY: Magic state surgery
CNOT_SURGERY: Perform two sequence lattice surgery
RELEASE: Deallocate logical qubit
```

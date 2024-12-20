cmake_minimum_required(VERSION 3.10)

project(QuantumSimulatorPipeline)

# Build and run routing
add_subdirectory(routing)
add_custom_target(run_routing
    COMMAND ${CMAKE_BINARY_DIR}/routing/program2 ../../circuit_synth/circuit/result_SELECT_7_Heisenberg2D_cylinder_0.5_0.5_3.raw ../routing/placement.txt result.txt 20 100
    #../../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_3.raw ../routing/placement.txt result.txt 13 100
    DEPENDS routing/program2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/routing
)

# Set the default target to run routing
add_custom_target(run_all
    DEPENDS run_routing
)





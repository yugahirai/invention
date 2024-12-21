cmake_minimum_required(VERSION 3.10)

project(QuantumSimulatorPipeline)

# Build and run placement
add_subdirectory(placement)
add_custom_target(run_placement
    COMMAND ${CMAKE_BINARY_DIR}/placement/program1 ../../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_5.raw ../routing/placement.txt 7 Heisenberg2D
    #../../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_3.raw ../routing/placement.txt 13 Heisenberg2D
    DEPENDS placement/program1
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/placement
)

# Build and run routing after placement
add_subdirectory(routing)
add_custom_target(run_routing
    COMMAND ${CMAKE_BINARY_DIR}/routing/program2 ../../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_2.raw ../routing/placement.txt result.txt 20 100
    #../../circuit_synth/circuit/result_SELECT_6_Heisenberg2D_cylinder_0.5_0.5_3.raw ../routing/placement.txt result.txt 13 100
    DEPENDS run_placement routing/program2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/routing
)

# Set the default target to run both
add_custom_target(run_all
    DEPENDS run_placement run_routing
)




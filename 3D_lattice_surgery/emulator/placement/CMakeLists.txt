cmake_minimum_required(VERSION 3.10)

project(QuantumSimulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    main.cpp
    load_file.cpp
    dump_file.cpp
    placement.cpp
    )

add_executable(program1 ${SOURCES})
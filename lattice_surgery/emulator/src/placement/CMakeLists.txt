cmake_minimum_required(VERSION 3.10)
file(GLOB SRC "*.c" "*.cpp" "*.hpp" "*.h")
add_executable(ftqc_placement ${SRC})

cmake_minimum_required(VERSION 3.10)

project(ftqc_evaluation LANGUAGES CXX)

set(WARNING_CPP "-Wall -Wextra")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
if(MSVC)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../bin)
endif()

if(MSVC)
	# ignore warning about template export and non-Unicode files
	add_compile_options(/wd4251)
	add_compile_options(/wd4819)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_CPP} -std=c++14")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native") # -mfpmath=both を削除
endif()

add_subdirectory(src)

if(MSVC)
	target_compile_options(ftqc_routing_visualize PRIVATE "/DFLAG_VISUALIZE")
else()
	target_compile_options(ftqc_routing_visualize PRIVATE "-DFLAG_VISUALIZE")
endif()

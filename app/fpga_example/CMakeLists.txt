cmake_minimum_required (VERSION 2.8.11)
project (opencl_examples)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )


include_directories ("${PROJECT_SOURCE_DIR}/../include" ${OpenCL_INCLUDE_DIRS})

add_subdirectory (sample) 
#include_directories ("${PROJECT_SOURCE_DIR}/include" ${OpenCL_INCLUDE_DIRS})

add_executable(fpga_sample1 sample1.c)
target_compile_features(fpga_sample1 PRIVATE cxx_range_for)
#target_link_libraries(fpga_sample1 ${OpenCL_LIBRARIES})


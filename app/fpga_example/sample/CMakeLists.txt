#include_directories ("${PROJECT_SOURCE_DIR}/include" ${OpenCL_INCLUDE_DIRS})

#add_executable(fpga_sample1 sample1.c)
#target_compile_features(fpga_sample1 PRIVATE cxx_range_for)
#target_link_libraries(fpga_sample1 ${OpenCL_LIBRARIES})


add_library(sample1 SHARED sample1.c)
target_compile_options(sample1 PUBLIC -Wall -Wextra )
set_target_properties(sample1 PROPERTIES ENABLE_EXPORTS on)
target_link_libraries(sample1  ${OpenCL_LIBRARIES})
set_property(TARGET sample1 PROPERTY LINK_FLAGS "-lOpenCL -shared")

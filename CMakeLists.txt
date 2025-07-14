cmake_minimum_required(VERSION 3.14)
project(polyfit C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Build polyfit as a static library so both the demo and tests can link it
add_library(polyfit STATIC polyfit.c)
target_include_directories(polyfit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(polyfit PUBLIC m)

enable_testing()
add_subdirectory(tests)

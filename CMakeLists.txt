cmake_minimum_required(VERSION 3.10)
project(polyfit_demo C)

set(CMAKE_C_STANDARD 11)

# Source files
set(SOURCES demo.c polyfit.c)

# Executable name
set(TARGET demo)

# Add the executable
add_executable(${TARGET} ${SOURCES})

# Link the math library
target_link_libraries(${TARGET} m)

cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 11)

# Source files
set(SOURCES demo.c polyFit.c)

# Executable name
set(TARGET demo)

# Add the executable
add_executable(${TARGET} ${SOURCES})

# Link the math library
target_link_libraries(${TARGET} m)

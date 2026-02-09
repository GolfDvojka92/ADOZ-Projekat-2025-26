cmake_minimum_required(VERSION 3.10)
project(AdaptiveSystems C)

# Require C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add executable
add_executable(adaptive
    main.c
    wav.c
    fir.c
    iir.c
    adaptive_filter.c
)

# Link math library (for sin, pow, etc.)
target_link_libraries(adaptive m)

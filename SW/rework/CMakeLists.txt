cmake_minimum_required(VERSION 3.10)
project(ADOZProject C)

# Require C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add executable
add_executable(zadatak1
    src/zadatak1.c
)
add_executable(zadatak2
    src/zadatak2.c
    src/wav.c
    src/fir.c
)
add_executable(zadatak3
    src/zadatak3.c
    src/wav.c
    src/fir.c
    src/direct_form_iir.c
    src/notch.c
)
add_executable(zadatak4
    src/zadatak4.c
    src/wav.c
    src/fir.c
    src/direct_form_iir.c
    src/notch.c
)

# Link math library (for sin, pow, etc.)
target_link_libraries(zadatak2 m)
target_link_libraries(zadatak3 m)
target_link_libraries(zadatak4 m)

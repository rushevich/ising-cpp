cmake_minimum_required(VERSION 3.15)

project(ising-cpp CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(ising-cpp)

target_sources(ising-cpp PUBLIC src/main.cpp)

target_sources(ising-cpp PRIVATE 
    src/lattice.h
    src/simulation.h
    src/csv.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
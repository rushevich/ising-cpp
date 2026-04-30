add_executable(ising-cpp ${SRC_DIR}/main.cpp ${SRC_DIR}/csv.cpp ${SRC_DIR}/cli.cpp
    ${SRC_DIR}/simulation.cpp)

target_compile_features(ising-cpp PUBLIC cxx_std_20)

target_include_directories(ising-cpp PUBLIC ${INC_DIR})
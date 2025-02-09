cmake_minimum_required(VERSION 3.10)
project(NBodySimulator)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(GTest REQUIRED)
# Main executable
add_executable(NBodySimulator
    N_body_simulator.cpp
    Number.cpp
    Body.cpp
    Button.cpp
    RK4.cpp
)
#Link SFML
target_link_libraries(NBodySimulator sfml-graphics sfml-window sfml-system)

# Test executable
add_executable(NumberClassTest number_class_test.cpp Number.cpp)
target_link_libraries(NumberClassTest GTest::GTest GTest::Main sfml-graphics sfml-window sfml-system)
enable_testing()
add_test(NAME NumberClassTest COMMAND NumberClassTest)
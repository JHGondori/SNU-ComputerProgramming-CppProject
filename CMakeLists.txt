cmake_minimum_required(VERSION 3.28)
project(NBodySimulator LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)
# Main executable
add_executable(NBodySimulator
    N_body_simulator.cpp
    Number.cpp
    Body.cpp
    Button.cpp
    RK4.cpp
)
target_compile_features(NBodySimulator PRIVATE cxx_std_17)
target_link_libraries(NBodySimulator PRIVATE SFML::Graphics SFML::Window SFML::System)
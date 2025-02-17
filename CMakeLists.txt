cmake_minimum_required(VERSION 3.10)
project(Mario2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "/usr/lib/cmake/SFML")


find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(mario src/main.cpp src/Level.cpp src/Player.cpp src/Entity.cpp src/Bloc.cpp src/Confetti.cpp)

target_link_libraries(mario sfml-graphics sfml-window sfml-system)


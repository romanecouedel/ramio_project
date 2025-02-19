cmake_minimum_required(VERSION 3.10)
project(Mario2D)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-install/lib/cmake/SFML")
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(mario src/main.cpp src/Level.cpp src/Player.cpp src/Entity.cpp src/Bloc.cpp src/Confetti.cpp src/Menu.cpp src/FinJeu.cpp src/Animation.cpp)

target_link_libraries(mario sfml-graphics sfml-window sfml-system)


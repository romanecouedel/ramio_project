cmake_minimum_required(VERSION 3.10)
project(Mario2D)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system audio)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(mario src/main.cpp src/Level.cpp src/Player.cpp src/Ennemi.cpp src/Entity.cpp src/Bloc.cpp src/Menu.cpp src/FinJeu.cpp src/Animation.cpp src/Audio.cpp)

target_link_libraries(mario sfml-graphics sfml-window sfml-system sfml-audio openal)

cmake_minimum_required(VERSION 3.16)
project(Pacman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Pacman
    src/main.cpp
    src/GameEngine.cpp
    src/Renderer.cpp
    src/Listener.cpp
)

target_include_directories(Pacman PRIVATE include)

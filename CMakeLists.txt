cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

file(GLOB project_SRCS src/*.cpp src/*.h)

project(Tamagochi)

add_executable(Tamagochi ${project_SRCS})

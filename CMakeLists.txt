cmake_minimum_required(VERSION 3.10)

project(MySimulation VERSION 1.0)

include_directories(include)

set(SOURCES
  src/main.c
  src/sim.c
)

add_executable(sandsim ${SOURCES})

cmake_minimum_required(VERSION 3.10)
project(SandSim VERSION 1.0)

find_package(raylib REQUIRED)

set(SOURCES
  src/main.c
  src/sim.c
)

add_executable(sand_sim ${SOURCES})

target_link_libraries(sand_sim raylib)

target_include_directories(sand_sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

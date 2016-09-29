cmake_minimum_required (VERSION 3.0.0)

PROJECT(draw)
add_executable(draw main.cpp world.cpp)

SET (CMAKE_MODULE_PATH "${draw_SOURCE_DIR}")

find_package (SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(draw ${SDL2_LIBRARY})

find_package (Agg REQUIRED)
INCLUDE(${AGG_USE_FILE})
target_link_libraries(draw ${AGG_LIBRARIES})

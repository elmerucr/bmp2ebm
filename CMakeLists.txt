cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")

project(bmp2ebm)

find_package(sdl2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    src/
)

add_subdirectory(src/)

add_executable(
    bmp2eb,
    src/main.cpp)

target_link_libraries(bmp2ebm ${SDL2_LIBRARIES})
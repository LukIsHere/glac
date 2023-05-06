cmake_minimum_required(VERSION 3.5)
project(scf VERSION 0.1.0)

add_executable(glac src/main.cpp src/glac.cpp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)
include_directories(include/dsl)
include_directories(include/scf)
include_directories(include/sw)

target_link_libraries(glac PUBLIC X11 pthread)
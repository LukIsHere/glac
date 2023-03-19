cmake_minimum_required(VERSION 3.5)
project(scf VERSION 0.1.0)

add_executable(scf test.cpp)

target_link_libraries(scf PUBLIC X11)
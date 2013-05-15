cmake_minimum_required (VERSION 2.6)
project (Tragopan)

set(executable_name Tragopan)
add_executable (Tragopan main.cpp)

add_definitions("-g")
add_definitions("-Wall")
add_definitions("-std=c++11")

target_link_libraries(Tragopan OgreMain)
target_link_libraries(Tragopan PolyVoxCore)
target_link_libraries(Tragopan PolyVoxUtil)



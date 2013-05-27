cmake_minimum_required (VERSION 2.6)
project (Tragopan)

set(executable_name Tragopan)

aux_source_directory(src/ FILES)

add_executable (Tragopan ${FILES})

#add_definitions("-g")
#add_definitions("-Wall")
add_definitions("-std=c++11")
add_definitions("-O3")

#TODO generalize me
include_directories(/usr/local/include/OGRE/)
include_directories(/usr/local/include/OGRE/Overlay/)
include_directories(/usr/local/include/PolyVoxCore/)
include_directories(/usr/include/OIS/)

target_link_libraries(Tragopan OgreMain)
target_link_libraries(Tragopan OgreOverlay)
target_link_libraries(Tragopan PolyVoxCore)
target_link_libraries(Tragopan PolyVoxUtil)
target_link_libraries(Tragopan OIS)
target_link_libraries(Tragopan boost_system)



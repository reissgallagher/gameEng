cmake_minimum_required(VERSION 3.11)
project(setup)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")

add_custom_target(copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res"
)

#### Practical 1 ####
add_executable(lab_1 practical_1/main.cpp)
target_include_directories(lab_1 PRIVATE ${SFML_INCS})
target_link_libraries(lab_1 sfml-graphics)

#### Practical 2 ####
add_executable(lab_2 practical_2/main.cpp)
target_include_directories(lab_2 PRIVATE ${SFML_INCS})
target_link_libraries(lab_2 sfml-graphics)  

#### Practical 3 ####
add_executable(space_invaders 
  practical_3/main.cpp
  practical_3/game_system.cpp
  practical_3/ship.cpp
  practical_3/bullet.cpp
)
target_include_directories(space_invaders PRIVATE ${SFML_INCS} practical_3)
target_link_libraries(space_invaders sfml-graphics)
add_dependencies(space_invaders copy_resources)

#BOX2D
add_subdirectory("lib/box2d")
set (B2D_INCS "lib/box2d/include")
link_directories("${CMAKE_BINARY_DIR}/lib/box2d/lib")

#physics 
add_executable(physics practical_5_physics/main.cpp)
target_include_directories(physics SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(physics box2d sfml-graphics)
set(EXECUTABLES ${EXECUTABLES} practical_5_physics)
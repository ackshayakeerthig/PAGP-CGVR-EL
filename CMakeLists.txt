
cmake_minimum_required(VERSION 3.16)

project(SceneAdaptiveCollisionDetection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(collision_sim
    src/main.cpp
    src/Collision.cpp
    src/SceneGenerator.cpp
    src/UniformGrid.cpp
    src/SpatialHash.cpp
    src/SweepAndPrune.cpp
)

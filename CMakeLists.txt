cmake_minimum_required(VERSION 3.14)
project(DungeonDescent CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DungeonDescent
    main.cpp
    src/combat/CombatSystem.cpp
)

target_include_directories(DungeonDescent PRIVATE ${CMAKE_SOURCE_DIR})

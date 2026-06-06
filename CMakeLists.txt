cmake_minimum_required(VERSION 3.14)
project(DungeonDescent CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download FTXUI automatically on first cmake run
include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG        v5.0.0
)
FetchContent_MakeAvailable(ftxui)

add_executable(DungeonDescent
    main.cpp
    src/combat/CombatSystem.cpp
    src/ui/UIManager.cpp
    src/save/SaveManager.cpp
)

target_include_directories(DungeonDescent PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(DungeonDescent
    PRIVATE ftxui::component
    PRIVATE ftxui::dom
    PRIVATE ftxui::screen
)

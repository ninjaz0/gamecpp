cmake_minimum_required(VERSION 3.24)

project(HollowStarter
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

add_executable(HollowStarter
    src/main.cpp
    src/assets/AssetManager.cpp
    src/characters/Enemy.cpp
    src/characters/Player.cpp
    src/core/Game.cpp
    src/core/Input.cpp
    src/level/Level.cpp
    src/physics/PhysicsSystem.cpp
    src/render/Animation.cpp
)

target_include_directories(HollowStarter
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(HollowStarter PRIVATE raylib)

if(APPLE)
    target_link_libraries(HollowStarter PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()

add_custom_command(TARGET HollowStarter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:HollowStarter>/assets
)

CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

project(survivor LANGUAGES C)

option(FETCH_RAYLIB "download and compile raylib for use in this project" OFF)

if(FETCH_RAYLIB)
    include(FetchContent)
    FetchContent_Declare(raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/6.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(raylib)
else()
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    message(WARNING "WARNING: Raylib was not found in the system")
    message(FATAL_ERROR "Run with -DFETCH_RAYLIB=ON to install raylib")
else()
    message(STATUS "SUCCESS: Found raylib in system")
endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" Force)
endif()

add_executable(${PROJECT_NAME} src/main.c src/utils.c)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

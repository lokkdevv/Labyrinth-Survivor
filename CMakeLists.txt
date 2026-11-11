CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

project(survivor LANGUAGES C)

option(FETCH_DEPENDENCIES
    "download and compile dependencies for use in this project" OFF)

if(FETCH_DEPENDENCIES)
    include(FetchContent)
    FetchContent_Declare(raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/6.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(raylib)
    #FetchContent_Declare(box2d
    #URL https://github.com/erincatto/box2d/archive/refs/tags/v3.0.0.tar.gz
    #DOWNLOAD_EXTRACT_TIMESTAMP ON
    #)
    #FetchContent_MakeAvailable(box2d)
else()
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    message(WARNING "WARNING: dependencies were not found in the system")
    message(FATAL_ERROR "Run with -DFETCH_DEPENDENCIES=ON to install them")
else()
    message(STATUS "SUCCESS: Found dependencies in system")
endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" Force)
endif()

add_executable(${PROJECT_NAME} src/main.c src/utils.c src/vector.c
                src/player.c src/collision.c)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib m)

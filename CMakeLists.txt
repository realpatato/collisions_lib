cmake_minimum_required(VERSION 3.28.3)

project(physics_lib)

add_executable(physics_test main.cpp)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 6.0
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 

FetchContent_MakeAvailable(raylib)

add_subdirectory(point)
add_subdirectory(constants)
add_subdirectory(shapes)
add_subdirectory(gjk)
add_subdirectory(epa)

target_link_libraries(physics_test
    PRIVATE 
        raylib
        point_lib
        shapes_lib
        gjk_lib
        epa_lib
        constants
)
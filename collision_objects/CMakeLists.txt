add_library(collision_objects_lib STATIC collision_objects.cpp collision_objects.hpp)

target_include_directories(collision_objects_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 6.0
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 

FetchContent_MakeAvailable(raylib)

target_link_libraries(collision_objects_lib
    PRIVATE
        point_lib
        shapes_lib
        gjk_lib
        epa_lib
        raylib
)
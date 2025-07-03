cmake_minimum_required(VERSION 3.31)
project(game C)

set(CMAKE_C_STANDARD 99)

add_executable(game src/main.c
        src/input.c
        src/input.h
        src/defs.h
        src/player.h
        src/player.c
        src/world.c
        src/world.h
)

include_directories(resources)
target_link_libraries(game raylib)

# === Copy resources folder on build ===
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(DEST_DIR "${CMAKE_BINARY_DIR}/resources")

add_custom_command(
        OUTPUT ${DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_DIR} ${DEST_DIR}
        COMMENT "Copying resources to build directory"
)

add_custom_target(copy_resources ALL DEPENDS ${DEST_DIR})
add_dependencies(game copy_resources)

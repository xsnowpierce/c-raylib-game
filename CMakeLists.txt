cmake_minimum_required(VERSION 3.22.1)
project(game C)

set(CMAKE_C_STANDARD 99)

add_executable(game src/main.c
        src/input.c
        src/input.h
        src/defs.h
        src/player/player.h
        src/player/player.c
        src/level/world.c
        src/level/world.h
        src/level/tilemap.c
        src/level/tilemap.h
        src/lib/parson.c
        src/lib/parson.h
        src/player/player_movement.c
        src/player/player_movement.h
        src/player/player_sprite.c
        src/player/player_sprite.h
        src/sprite/sprite.h
        src/player/player_combat.c
        src/player/player_combat.h
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

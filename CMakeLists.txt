cmake_minimum_required(VERSION 3.31)
project(game C)

set(CMAKE_C_STANDARD 99)

add_executable(game main.c
        input.c
        input.h
        defs.h
        player.h
        player.c
        world.c
        world.h
)

include_directories(resources)
target_link_libraries(game raylib)

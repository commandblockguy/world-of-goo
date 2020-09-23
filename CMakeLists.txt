cmake_minimum_required(VERSION 3.15)
project(tanks C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "-S -nostdinc -isystem ~/CEdev/include -Dinterrupt=\"__attribute__\(\(__interrupt__\)\)\" -Dreentrant= -D_EZ80 -Wno-main-return-type")

include_directories($ENV{CEDEV}/include)

add_custom_target(Make ALL $(MAKE) WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# A dummy target to make CLion do stuff
add_executable(dummy
        src/gooball.c
        src/gooball.h
        src/util.c
        src/util.h
        src/main.c)
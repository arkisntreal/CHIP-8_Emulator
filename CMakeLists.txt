cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(CHIP-8_Emulator)

set(SOURCE_FILES
    src/chip8.cpp
    src/window.cpp
    src/font.cpp    
)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_executable(CHIP-8_Emulator main.cpp ${SOURCE_FILES})

if (TARGET SDL2::SDL2main)
    target_link_libraries(CHIP-8_Emulator PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(CHIP-8_Emulator PRIVATE SDL2::SDL2)


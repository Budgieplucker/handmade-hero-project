cmake_minimum_required(VERSION 3.25)

project(handmade-hero-project)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set(VS_STARTUP_PROJECT handmade-hero-project)

include_directories(headers)

add_executable(handmade-hero-project WIN32
    src/handmade.cpp)

target_link_libraries(handmade-hero-project PUBLIC user32.lib gdi32.lib)
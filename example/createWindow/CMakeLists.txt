cmake_minimum_required(VERSION 3.10)

project(creareWindow LANGUAGES CXX)

add_definitions(-DFREEGUI_PLATFORM_WINDOWS)

add_executable(creareWindow
    main.cpp
    ../../src/core//object.cpp
    ../../src/core/signal_slot.cpp
    ../../src/widgets/widget.cpp
    ../../src/widgets/window.cpp
)

target_include_directories(creareWindow PUBLIC "../../include/FreeGUI")

cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_DISABLE_PRECOMPILE_HEADERS 1)

project(drag-drop VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED 
    src/mac.mm
    src/win.cpp
    src/DragDrop.cpp
)
set(CMAKE_PRE)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
endif()
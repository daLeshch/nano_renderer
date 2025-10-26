cmake_minimum_required(VERSION 3.21)
project(nanorenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.24
)
FetchContent_MakeAvailable(SDL)

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

add_executable(nanorenderer
    lib/tgaimage.cpp
    lib/tgaimage.h
    src/main.cpp
    src/model.h
    src/render.h
    src/render.cpp
    src/math_core.h
)
target_include_directories(nanorenderer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(nanorenderer PRIVATE SDL3::SDL3)

add_custom_command(TARGET nanorenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:nanorenderer>"
)

if(MSVC)
    add_compile_definitions(_USE_MATH_DEFINES)
endif()

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(nanorenderer PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
        ${OPENGL_LIBRARY}
    )
endif()

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(nanorenderer PRIVATE X11)
endif()

add_executable(tests
    src/tests.cpp
    src/math_core.h
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

set_target_properties(tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)


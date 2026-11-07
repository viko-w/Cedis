cmake_minimum_required(VERSION 3.20)
project(cedis)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glm CONFIG REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    #src/clock.cpp
    #src/tui.cpp

    src/render/render.cpp
    src/render/shader.cpp
    src/render/texture.cpp
    src/render/camera.cpp
    src/render/mesh.cpp
    src/render/model.cpp
    src/render/vbo.cpp
    src/render/ebo.cpp
    src/render/vao.cpp
    
    src/include/gl.c
    src/include/stb.cpp
)

include(FetchContent)
FetchContent_Declare(
    cpptui
    GIT_REPOSITORY https://github.com/jonoton/cpp-tui.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(cpptui)

add_executable(cedis ${SOURCE_FILES})

target_include_directories(cedis PRIVATE src/include)
target_link_libraries(cedis PRIVATE cpptui::cpptui glfw GL)

file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json
                 ${CMAKE_SOURCE_DIR}/compile_commands.json SYMBOLIC)
cmake_minimum_required(VERSION 3.20)
project(cedis)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
    src/main.cpp
    src/clock.cpp
    src/tui.cpp
)

include(FetchContent)
FetchContent_Declare(
    cpptui
    GIT_REPOSITORY https://github.com/jonoton/cpp-tui.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(cpptui)

add_executable(cedis ${SOURCE_FILES})

target_link_libraries(cedis PRIVATE cpptui::cpptui)

file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json
                 ${CMAKE_SOURCE_DIR}/compile_commands.json SYMBOLIC)
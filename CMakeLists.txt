cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PECULIAR LANGUAGES CXX)

add_subdirectory(vendor)

add_executable(
    PECULIAR
    "src/renderer/renderer2d.h"
    "src/renderer/renderer2d.cpp"
    "src/renderer/shader.h"
    "src/renderer/shader.cpp"
    "src/renderer/texture.h"
    "src/renderer/texture.cpp"
    "src/renderer/vertexarray.h"
    "src/renderer/vertexarray.cpp"
    "src/renderer/vertexbuffer.h"
    "src/renderer/vertexbuffer.cpp"
    "src/main.cpp"
)

target_include_directories(
    PECULIAR PRIVATE
    "src/"
)

target_link_libraries(
    PECULIAR PRIVATE
    glad
    glfw
    spdlog
    glm::glm
)
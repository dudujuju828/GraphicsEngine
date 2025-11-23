
cmake_minimum_required(VERSION 3.20)

project(OpenGL)
message("Project Name: ${PROJECT_NAME}")
message("Cmake Source Dir: ${CMAKE_SOURCE_DIR}")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
message("IMGUI_DIR: ${IMGUI_DIR}")

add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backend)

add_executable(main src/main.cpp src/window.cpp src/glad.c)
target_include_directories(main PRIVATE include/)
target_link_libraries(main spdlog fmt glfw imgui)

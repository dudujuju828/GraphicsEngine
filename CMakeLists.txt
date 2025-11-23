
cmake_minimum_required(VERSION 3.20)

project(OpenGL)
message("Project Name: ${PROJECT_NAME}")

add_executable(main src/main.cpp src/window.cpp src/glad.c)
target_include_directories(main PRIVATE include/)
target_link_libraries(main spdlog fmt glfw libGL)

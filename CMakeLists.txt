cmake_minimum_required(VERSION 3.20)

project(OpenGL)

# Use C++17 everywhere
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prefer static libs by default
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

message("Project Name: ${PROJECT_NAME}")
message("Cmake Source Dir: ${CMAKE_SOURCE_DIR}")

set(IMGUI_DIR  ${CMAKE_SOURCE_DIR}/external/imgui)
set(GLM_DIR    ${CMAKE_SOURCE_DIR}/external/glm)
set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/external/assimp)
set(GLFW_DIR   ${CMAKE_SOURCE_DIR}/external/glfw)
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/external/spdlog)

# -------- Assimp options: build static, no tools/tests --------
set(ASSIMP_BUILD_TESTS         OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS  OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL             OFF CACHE BOOL "" FORCE)
# Main switch Assimp honours for shared vs static:
set(ASSIMP_BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE) # if present
# And/or just rely on BUILD_SHARED_LIBS=OFF

# -------- GLFW options: force static --------
set(GLFW_LIBRARY_TYPE STATIC CACHE STRING "STATIC" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)

# -------- spdlog: header-only / no shared --------
set(SPDLOG_BUILD_SHARED  OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)

add_subdirectory(${ASSIMP_DIR})
add_subdirectory(${GLFW_DIR})
add_subdirectory(${SPDLOG_DIR})

# -------- ImGui: build our own static lib --------
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

# -------- main executable --------
add_executable(main
    src/main.cpp
    src/engine.cpp
    src/window.cpp
    src/shader.cpp
    src/mesh.cpp
    src/glad.c
    src/object.cpp
    src/camera.cpp
    src/terrain.cpp
    src/grass.cpp
    src/texture_manager.cpp
)

target_include_directories(main PRIVATE
    include/
    ${GLM_DIR}
)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
FetchContent_Declare(
    nativefiledialog
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG        v1.2.1
)
FetchContent_MakeAvailable(googletest nativefiledialog)
enable_testing()
add_executable(camera_tests tests/camera_test.cpp src/camera.cpp)
target_include_directories(camera_tests PRIVATE include ${GLM_DIR})
target_link_libraries(camera_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(camera_tests)

target_link_libraries(main PRIVATE
    spdlog::spdlog_header_only   # header-only, no DLL
    glfw
    imgui
    assimp
    nfd
)
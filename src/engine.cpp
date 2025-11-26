
#include "../include/glad/glad.h"
#include "../include/mesh.hpp"
#include "../include/engine.hpp"
#include "../include/object.hpp"
#include "../include/shader.hpp"
#include "../include/terrain.hpp"

#include <string_view>
#include <filesystem>
#include <vector>

#include "imgui.h"
#include "../external/PerlinNoise/PerlinNoise.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(std::string_view title) : window(title), camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
    GLFWwindow* glfwWin = window.getHandle();

    int width, height;
    glfwGetFramebufferSize(glfwWin, &width, &height);
    camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    glViewport(0, 0, width, height);

    window.setCursorCaptured(true);

    double xpos, ypos;
    glfwGetCursorPos(glfwWin, &xpos, &ypos);
    lastMouseX = static_cast<float>(xpos);
    lastMouseY = static_cast<float>(ypos);
    firstMouse = false;
}

void Engine::processMouse() {
	
	if (!mouseCaptured) return;

    GLFWwindow* glfwWin = window.getHandle();

    double xposD, yposD;
    glfwGetCursorPos(glfwWin, &xposD, &yposD);

    float xpos = static_cast<float>(xposD);
    float ypos = static_cast<float>(yposD);

    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed: y goes down in screen coords

    lastMouseX = xpos;
    lastMouseY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void Engine::run() {
    GLFWwindow* glfwWin = window.getHandle();

    Shader shader("src/vertex_s.glsl", "src/fragment_s.glsl");

    const siv::PerlinNoise::seed_type seed = 123456u;
    siv::PerlinNoise perlin{ seed };

	float terrainAmplitude = 5.0f;
    Mesh terrainMesh = createTerrainMesh(
        200, 200,        // xSegments, zSegments
        100.0f, 100.0f,  // sizeX, sizeZ
        perlin,
        0.03,            // frequency
        6,               // octaves
        terrainAmplitude             // amplitude
    );

    Object terrain{ std::move(terrainMesh) };
    terrain.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

    lastFrameTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(glfwWin)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime   = currentTime - lastFrameTime;
        lastFrameTime     = currentTime;

        window.pollEvents();
        processInput(deltaTime);
        processMouse();

        window.setColor(0.1f, 0.1f, 0.15f);

        shader.useProgram();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());

		shader.setFloat("uMinHeight", -terrainAmplitude);
		shader.setFloat("uMaxHeight", terrainAmplitude);


        terrain.draw(shader);

        window.swapBuffers();
    }
}

void Engine::processInput(float deltaTime) {
    GLFWwindow* glfwWin = window.getHandle();
	
	if (glfwGetKey(glfwWin, GLFW_KEY_M) == GLFW_PRESS) {
		if (!mouseCaptured) {
			window.setCursorCaptured(true);
			mouseCaptured = true;
			firstMouse = true; // recenter offsets next time
		}
	}
	if (glfwGetKey(glfwWin, GLFW_KEY_N) == GLFW_PRESS) {
		if (mouseCaptured) {
			window.setCursorCaptured(false);
			mouseCaptured = false;
		}
	}


    if (glfwGetKey(glfwWin, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWin, true);

    if (glfwGetKey(glfwWin, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Forward, deltaTime);
    if (glfwGetKey(glfwWin, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Backward, deltaTime);
    if (glfwGetKey(glfwWin, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Left, deltaTime);
    if (glfwGetKey(glfwWin, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Right, deltaTime);
}


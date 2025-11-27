
#include "../include/glad/glad.h"
#include "../include/mesh.hpp"
#include "../include/engine.hpp"
#include "../include/object.hpp"
#include "../include/shader.hpp"
#include "../include/terrain.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"


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
    Shader model("src/model_vs.glsl", "src/model_fs.glsl");

    const siv::PerlinNoise::seed_type seed = 123456u;
    siv::PerlinNoise perlin{ seed };

	float terrainAmplitude = 5.0f;
    Mesh terrainMesh = createTerrainMesh(
        200, 200,        // xSegments, zSegments
        100.0f, 100.0f,  // sizeX, sizeZ
        perlin,
        0.02,            // frequency
        4,               // octaves
        terrainAmplitude             // amplitude
    );

    Object terrain{ std::move(terrainMesh) };
    terrain.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

    std::filesystem::path modelPath = "assets/models/bunny.obj"; 
    Object modelObject{modelPath};  	

	glEnable(GL_DEPTH_TEST);
    lastFrameTime = static_cast<float>(glfwGetTime());

	/*ENCAPSULATE SOMEWHERHE ELSE - DONT FORGET */
	unsigned int grassTex;
    glGenTextures(1, &grassTex);
    glBindTexture(GL_TEXTURE_2D, grassTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("assets/textures/grass.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                     format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    while (!glfwWindowShouldClose(glfwWin)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime   = currentTime - lastFrameTime;
        lastFrameTime     = currentTime;

        window.pollEvents();
        processInput(deltaTime);
        processMouse();

        window.setColor(0.1f, 0.1f, 0.15f);
	
		glm::mat4 projection = camera.getProjectionMatrix();
		glm::mat4 view = camera.getViewMatrix();

        shader.useProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTex);
		shader.setInt("uGrassTexture",0);
		shader.setFloat("uTexScale", 0.1f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
		shader.setFloat("uMinHeight", -terrainAmplitude);
		shader.setFloat("uMaxHeight", terrainAmplitude);
        terrain.draw(shader);

		model.useProgram();
        model.setMat4("view", view);
        model.setMat4("projection", projection);
		model.setVec3("lightDir", glm::vec3(-0.5f,-1.0f,-0.3f));	
		model.setVec3("lightColor", glm::vec3(1.0f,1.0f,1.0f));	
		model.setVec3("objectColor", glm::vec3(0.9f,0.8f,0.7f));	
		//modelObject.draw(model);
	
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


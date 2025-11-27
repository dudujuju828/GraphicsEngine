#include "../include/glad/glad.h"
#include "../include/mesh.hpp"
#include "../include/engine.hpp"
#include "../include/object.hpp"
#include "../include/shader.hpp"
#include "../include/terrain.hpp"
#include "../include/grass.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <string_view>
#include <filesystem>
#include <vector>

#include "imgui.h"
#include "../external/PerlinNoise/PerlinNoise.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(std::string_view title) : window(title), camera(glm::vec3(0.0f, 8.0f, 0.0f)) {
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
    float yoffset = lastMouseY - ypos;

    lastMouseX = xpos;
    lastMouseY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void Engine::run() {
    GLFWwindow* glfwWin = window.getHandle();

	/* setting up imgui start */

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; 

    ImGui::StyleColorsDark();
	io.FontGlobalScale = 6.0f;            
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(3.0f);
    ImGui_ImplGlfw_InitForOpenGL(glfwWin, true);
    ImGui_ImplOpenGL3_Init("#version 330");

	/* setting up imgui end */

    Shader shader("src/vertex_s.glsl", "src/fragment_s.glsl");
    Shader model("src/model_vs.glsl", "src/model_fs.glsl");
    Shader grassShader("src/grass_vs.glsl", "src/grass_fs.glsl");

    const siv::PerlinNoise::seed_type seed = 123456u;
    siv::PerlinNoise perlin{ seed };

    unsigned int terrainXSegments = 200;
    unsigned int terrainZSegments = 200;
    float terrainSizeX = 100.0f;
    float terrainSizeZ = 100.0f;
    double terrainFrequency = 0.04f;
    int terrainOctaves = 4;
    float terrainAmplitude = 5.0f;

    Mesh terrainMesh = createTerrainMesh(
        terrainXSegments,
        terrainZSegments,
        terrainSizeX,
        terrainSizeZ,
        perlin,
        terrainFrequency,
        terrainOctaves,
        terrainAmplitude
    );

    Object terrain{ std::move(terrainMesh) };
    terrain.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);

    std::filesystem::path modelPath = "assets/models/bunny.obj";
    Object modelObject{modelPath};

    std::filesystem::path treePath = "assets/models/tree.obj";
    Object treeObject{treePath};
	treeObject.setScale(glm::vec3(0.4f));
	treeObject.setPosition(glm::vec3(-20.0f,0.0f,20.0f));

    GrassField grass;
    glm::vec2 xRange(-terrainSizeX * 0.5f, terrainSizeX * 0.5f);
    glm::vec2 zRange(-terrainSizeZ * 0.5f, terrainSizeZ * 0.5f);
	auto sampleHeight = [&](float xWorld, float zWorld) -> float {
		double nx = static_cast<double>(xWorld) * terrainFrequency;
		double nz = static_cast<double>(zWorld) * terrainFrequency;
		double h = perlin.octave2D_01(nx, nz, terrainOctaves);
		h = h * 2.0 - 1.0;
		return static_cast<float>(h * terrainAmplitude);
	};


	int grassBladeCount = 100000;
    grass.init(grassBladeCount, xRange, zRange, sampleHeight);

    glEnable(GL_DEPTH_TEST);
    lastFrameTime = static_cast<float>(glfwGetTime());
	unsigned int terrainTex;
	glGenTextures(1, &terrainTex);
	glBindTexture(GL_TEXTURE_2D, terrainTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/textures/grass.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	unsigned int grassBladeTex;
	glGenTextures(1, &grassBladeTex);
	glBindTexture(GL_TEXTURE_2D, grassBladeTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("assets/textures/grass_blade.png", &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	glfwSwapInterval(0);

	float currentFPS = 0.0f;
	float averageFPS = 0.0f;
	float totalTime = 0.0f;
	int frameCount = 0;
	static float grassDensity = 0.5f;  // Default value

    while (!glfwWindowShouldClose(glfwWin)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        window.pollEvents();
        processInput(deltaTime);
        processMouse();

		/* imguiframe stuff */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		
		currentFPS = 1.0f/deltaTime;
		totalTime += deltaTime;
		frameCount ++;
		
		if (totalTime >= 1.0f) {
			averageFPS = frameCount / totalTime;
			totalTime = 0.0f;
			frameCount = 0;
		}	

		ImVec2 newWindowSize = ImVec2(1500, 600);  // Set the width to 500 and height to 300
		ImGui::SetNextWindowSize(newWindowSize, ImGuiCond_Always);

        ImGui::Begin("Stats");
        ImGui::Text("Current FPS: %.1f", currentFPS);
        ImGui::Text("Average FPS: %.1f", averageFPS);
        ImGui::Text("Frame time: %.3f ms", deltaTime * 1000.0f);
        ImGui::Text("Grass blades: %u", grassBladeCount);
		/* imguiframe stuff end */


        window.setColor(0.361f, 0.6f, 1.0f);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();

        shader.useProgram();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, terrainTex);
        shader.setInt("uGrassTexture", 0);
        shader.setFloat("uTexScale", 0.1f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat("uMinHeight", -terrainAmplitude);
        shader.setFloat("uMaxHeight", terrainAmplitude);
		shader.setVec3("uLightDir", glm::vec3(-0.5f, -1.0f, -0.3f));
		shader.setVec3("uLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("uAmbientColor", glm::vec3(0.25f, 0.3f, 0.35f));
        terrain.draw(shader);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glDisable(GL_CULL_FACE);
		if (ImGui::SliderFloat("Grass Density", &grassDensity, 0.0f, 1.0f)) {
			// Adjust grass density based on slider value
			int newBladeCount = static_cast<int>(grassDensity * 1000000);  // Adjust scaling factor as needed
			grassBladeCount = newBladeCount;
			grass.init(grassBladeCount, xRange, zRange, sampleHeight);  // Re-initialize grass with new blade count
		}		
        grass.draw(grassShader, view, projection, currentTime, grassBladeTex);
        //glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        ImGui::End();
		/* imguiframe stuff end */


        model.useProgram();
        model.setMat4("view", view);
        model.setMat4("projection", projection);
        model.setVec3("lightDir", glm::vec3(-0.5f, -1.0f, -0.3f));
        model.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        model.setVec3("objectColor", glm::vec3(0.43f, 0.21f, 0.08f));
		treeObject.draw(model);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Engine::processInput(float deltaTime) {
    GLFWwindow* glfwWin = window.getHandle();

    if (glfwGetKey(glfwWin, GLFW_KEY_M) == GLFW_PRESS) {
        if (!mouseCaptured) {
            window.setCursorCaptured(true);
            mouseCaptured = true;
            firstMouse = true;
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


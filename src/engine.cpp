
#include "../include/glad/glad.h"
#include "../include/mesh.hpp"
#include "../include/engine.hpp"
#include "../include/object.hpp"
#include "../include/shader.hpp"

#include <string_view>
#include <filesystem>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(std::string_view s) : window(s), camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
		GLFWwindow* win = window.getHandle();
		int width, height;
		glfwGetFramebufferSize(win,&width,&height);
		camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));	
}

void Engine::run() {

	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	static bool show_wireframe = false;
	*/
	Shader shader("src/vertex_s.glsl","src/fragment_s.glsl");
	Object triangle("src/triangle.obj");

	lastFrameTime = static_cast<float>(glfwGetTime());

	
	while (window.isOpen()) {
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		
		window.pollEvents();
		processInput(deltaTime);


		window.setColor(0.0f,0.5f,0.5f);
		shader.useProgram();
		
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("projection", camera.getProjectionMatrix());

		triangle.draw(shader);

		window.swapBuffers();

		/*ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, ImGui");
		ImGui::Checkbox("Wireframe", &show_wireframe);
		ImGui::Text("If you see this, ImGui is working.");
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		*/	
	}
}

void Engine::processInput(float deltaTime) {
    GLFWwindow* glfwWin = window.getHandle();

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


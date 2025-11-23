
#include "../include/glad/glad.h"
#include "../include/engine.hpp"
#include <string_view>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(std::string_view s) : window(s) {}

void Engine::run() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	static bool show_wireframe = false;
	while (window.isOpen()) {
		window.pollEvents();
		
		glClearColor(0.5f,0.5f,0.5f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// minimal test window
		ImGui::Begin("Hello, ImGui");
		ImGui::Checkbox("Wireframe", &show_wireframe);
		ImGui::Text("If you see this, ImGui is working.");
		ImGui::End();

		// render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		window.swapBuffers();
	}
	
}

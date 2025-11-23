
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_opengl3.h"


int main() {


	Window window("Graphics");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	while (window.isOpen()) {
		window.pollEvents();
		
		glClearColor(0.5f,0.5f,0.5f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// minimal test window
		ImGui::Begin("Hello, ImGui");
		ImGui::Text("If you see this, ImGui is working.");
		ImGui::End();

		// render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		window.swapBuffers();
	}

	return 0;
}


#include "../include/glad/glad.h"
#include "../include/mesh.hpp"
#include "../include/engine.hpp"
#include "../include/shader.hpp"

#include <string_view>
#include <filesystem>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Engine::Engine(std::string_view s) : window(s) {}

void Engine::run() {

	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	static bool show_wireframe = false;
	*/
	std::filesystem::path vpath("src/vertex_s.glsl");
	std::filesystem::path fpath("src/fragment_s.glsl");
	Shader shader(vpath,fpath);
	glUseProgram(shader.getID());
	
	Mesh triangle("src/triangle.obj");

	
	while (window.isOpen()) {
		window.setColor(0.0f,0.5f,0.5f);



		window.pollEvents();
		

		triangle.draw();

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

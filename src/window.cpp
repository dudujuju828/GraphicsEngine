
#include "../include/window.hpp"

#include <string>
#include <string_view>
#include <cstddef>
#include <iostream>


#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

Window::~Window(){};

Window::Window(std::string_view window_name) {
	if (!glfwInit()) {
		std::cerr << "Error: glfwInit()\n";
	} else {
		spdlog::info("Success: glfwInit()");
	}

	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* primary_mode = glfwGetVideoMode(primary_monitor);
	int primary_width = primary_mode->width;	
	int primary_height = primary_mode->height;	
	std::cout << "Success: glfwInit()\n";
	
	window_handle = glfwCreateWindow(primary_width, primary_height, window_name.data(), nullptr, nullptr);
}

GLFWwindow* Window::get_handle() {
	return window_handle;
}

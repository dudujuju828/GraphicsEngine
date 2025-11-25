
#include <string>
#include <string_view>
#include <cstddef>
#include <iostream>


#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "../include/window.hpp"

Window::~Window(){
	glfwDestroyWindow(window_handle);
	spdlog::warn("Window destroyed.");
	glfwTerminate();
	spdlog::warn("GLFW terminated.");
};

Window::Window(std::string_view window_name) {
	if (!glfwInit()) {
		spdlog::error("GLFW failed to initialize.");
	} else {
		spdlog::info("GLFW successfully initialized.");
	}

	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* primary_mode = glfwGetVideoMode(primary_monitor);
	int primary_width = primary_mode->width;	
	int primary_height = primary_mode->height;	
	window_handle = glfwCreateWindow(primary_width, primary_height, window_name.data(), nullptr, nullptr);

	int framebuffer_width, framebuffer_height;
	glfwGetFramebufferSize(window_handle,&framebuffer_width,&framebuffer_height);

	if (window_handle) {
		spdlog::info("Window created from monitor of width {} and height {}.",primary_width,primary_height);
		spdlog::info("Window has framebuffer width {} with framebuffer height {}.",primary_width,primary_height);
	}
	
	glfwMakeContextCurrent(window_handle);
	spdlog::info("OpenGL context of window named {} has been made current.",window_name);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("GLAD unable to load function pointers.");	
	} else {
		spdlog::info("OpenGL function pointers successfully loaded.");
	}

	glfwSetFramebufferSizeCallback(window_handle, framebuffer_size_callback);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	spdlog::info("Framebuffer resizing to width {} and height {}.",width,height);
	glViewport(0, 0, width, height);	
}

GLFWwindow* Window::getHandle() {
	return window_handle;
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(window_handle);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::setColor(float r, float g, float b) {
	glClearColor(r,g,b,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window_handle);
}

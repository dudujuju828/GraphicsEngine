
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

int main() {


	Window window("Graphics");
	GLFWwindow* window_handle = window.getHandle();

	while (!glfwWindowShouldClose(window_handle)) {
		
		glfwSwapBuffers(window_handle);
		glfwPollEvents();	
	}

	glfwTerminate();
	return 0;
}


#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

int main() {


	Window window("Graphics");
	GLFWwindow* window_handle = window.getHandle();

	while (window.isOpen()) {
		
		glfwSwapBuffers(window_handle);
		glfwPollEvents();	
	}

	glfwTerminate();
	return 0;
}

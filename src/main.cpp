
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

int main() {


	Window window("mywindow");
	GLFWwindow* window_handle = window.get_handle();

	glfwMakeContextCurrent(window_handle);

	while (!glfwWindowShouldClose(window_handle)) {
		
		glfwSwapBuffers(window_handle);
		glfwPollEvents();	
	}

	glfwTerminate();
	return 0;
}

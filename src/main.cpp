
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

int main() {


	Window window("mywindow");

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		
		glfwSwapBuffers(window);
		glfwPollEvents();	
	}

	glfwTerminate();
	return 0;
}

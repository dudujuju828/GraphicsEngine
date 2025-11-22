
#include <GLFW/glfw3.h>
#include <iostream>

int main() {

	if (!glfwInit()) {
		std::cerr << "ERROR: glfwInit() failed.\n";
		return -1;
	} else {
		std::cout << "GLFW Initialized.\n";
	}

	GLFWwindow* window = glfwCreateWindow(800,600,"TITLE",nullptr,nullptr);
	if (window) {
		std::cout << "Window created.\n";
	} else {
		std::cerr << "Failed to create window.\n";
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		
		glfwSwapBuffers(window);
		glfwPollEvents();	
	}

	glfwTerminate();
	return 0;
}

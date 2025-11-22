
#include <GLFW/glfw3.h>
#include <iostream>

int main() {

	if (!glfwInit()) {
		std::cerr << "ERROR: glfwInit() failed.\n"
		return -1;
	} else {
		std::cout << "GLFW Initialized.\n"
	}
	
}

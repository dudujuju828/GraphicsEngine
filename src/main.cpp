
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

int main() {


	Window window("Graphics");

	while (window.isOpen()) {
		window.pollEvents();
		
		
		window.swapBuffers();
	}

	return 0;
}

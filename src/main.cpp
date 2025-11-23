
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"

int main() {


	Window window("Graphics");

	while (window.isOpen()) {
		window.pollEvents();
		
		glClearColor(0.5f,0.5f,0.5f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		window.swapBuffers();
	}

	return 0;
}

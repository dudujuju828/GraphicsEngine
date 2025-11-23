
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/window.hpp"
#include "../include/engine.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


int main() {


	Engine engine("Graphics");
	engine.run();

	return 0;
}

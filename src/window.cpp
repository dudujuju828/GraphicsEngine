
#include "../include/window.hpp"
#include <string>
#include <string_view>
#include <cstdef>

Window::Window(std::string_view window_name) :
			  {
					if (!glfwInit()) {
						std::cerr << "Error: glfwInit()\n";
					} else {
						std::cout << "Success: glfwInit()\n";

					GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
					GLFWvidmode* primary_mode = glfwGetVideoMode(primary_monitor);
					int primary_width = primary_mode->width;	
					int primary_height = primary_mode->height;	
					
					window_handle = glfwCreateWindow(primary_width, primary_height, window_name, nullptr, nullptr);
			  }

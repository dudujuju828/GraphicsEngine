
#ifndef SRC_WINDOW_HPP
#define SRC_WINDOW_HPP

#include <string>
#include <cstdef>
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow* window_handle;

public:
	~Window();
	Window(std::string window_name);
	GLFWwindow* get_handle();
};

#endif

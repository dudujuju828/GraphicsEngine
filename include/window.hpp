
#ifndef SRC_WINDOW_HPP
#define SRC_WINDOW_HPP

#include <string>
#include <cstdef>
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow* windowHandle;

public:
	~Window();
	Window(std::string window_name, std::size_t width, std::size_t height);
};

#endif

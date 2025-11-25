
#ifndef SRC_WINDOW_HPP
#define SRC_WINDOW_HPP

#include <string_view>
#include <cstddef>
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow* window_handle;

public:
	~Window();
	Window(std::string_view window_name);
	GLFWwindow* getHandle();
	bool isOpen();
	void pollEvents();
	void swapBuffers();
	void setColor(float r, float g, float b);
	void setCursorCaptured(bool captured);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif


#ifndef SRC_ENGINE_HPP
#define SRC_ENGINE_HPP

#include "window.hpp"
#include "camera.hpp"
#include <string_view>

class Engine {
	private:
	Window window;	
	Camera camera;
	float lastFrameTime = 0.0f;
	void processInput(float deltaTime);

	bool firstMouse;
	bool mouseCaptured = true;
	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;
	void processMouse();
	
	public:
	Engine(std::string_view s);
	void run();

};

#endif


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
	
	public:
	Engine(std::string_view s);
	void run();

};

#endif

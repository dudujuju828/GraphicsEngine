
#ifndef SRC_ENGINE_HPP
#define SRC_ENGINE_HPP

#include "window.hpp"
#include <string_view>

class Engine {
	private:
	Window window;	
	public:
	Engine(std::string_view s);
	void run();

};

#endif


#ifndef SRC_MESH_HPP
#define SRC_MESH_HPP

#include "../include/glad/glad.h"
#include <filesystem>
#include <vector>

class Mesh {
	private:
	GLuint VBO;
	GLuint VAO;
	
	public:
	Mesh(std::filesystem::path objfile_path);
	void use();
	void draw();

};

#endif 

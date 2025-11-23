
#include "../include/shader.hpp"
#include "../include/glad/glad.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>

Shader::Shader(std::filesystem::path &vertex_path, std::filesystem::path &fragment_path) {

	GLuint vertex_id = compileShader(loadFromFile(vertex_path), GL_VERTEX_SHADER);
	GLuint fragment_id = compileShader(loadFromFile(fragment_path), GL_FRAGMENT_SHADER);
	createProgram(vertex_id, fragment_id);
}

void Shader::createProgram(GLuint vertex_id, GLuint fragment_id) {
	programID = glCreateProgram();
	glAttachShader(programID, vertex_id);
	glAttachShader(programID, fragment_id);
	glLinkProgram(programID);
	
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

GLuint Shader::compileShader(const std::string& source, GLenum shader_type) {
	GLuint shader_id = glCreateShader(shader_type);	
	const char* src = source.c_str();
	
	glShaderSource(shader_id,1,&src,nullptr);
	glCompileShader(shader_id);
	
	return shader_id;
}

std::string Shader::loadFromFile(const std::filesystem::path path) {

	spdlog::info("File at {} is being read into string.",path.string());	

	if (!std::filesystem::exists(path)) {
		spdlog::error("File path is not found: {}",path.string());
		throw std::runtime_error("Path does not exist: " + path.string());
	}


	std::ifstream file(path);

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string file_contents = buffer.str();	

	return file_contents;
}

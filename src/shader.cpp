
#include "../include/shader.hpp"
#include "../include/glad/glad.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path) {

	GLuint vertex_id = compileShader(loadFromFile(vertex_path), GL_VERTEX_SHADER);
	GLuint fragment_id = compileShader(loadFromFile(fragment_path), GL_FRAGMENT_SHADER);
	createProgram(vertex_id, fragment_id);
}

GLuint Shader::getID() {
	return programID;
}

void Shader::useProgram() {
	spdlog::info("useProgram called with program ID: {}",programID);
	glUseProgram(programID);
}

void Shader::createProgram(GLuint vertex_id, GLuint fragment_id) {
	programID = glCreateProgram();
	glAttachShader(programID, vertex_id);
	glAttachShader(programID, fragment_id);
	glLinkProgram(programID);
	
	GLint status = 0;	
	glGetProgramiv(programID,GL_LINK_STATUS,&status);
	
	if (status) {
		spdlog::info("Program linking was successful.");
	} else {
		char bufferLog[512];
		glGetProgramInfoLog(programID,512,nullptr,bufferLog);
		spdlog::error("Program linking was unsuccessful: {}",bufferLog);
	}

	
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

void Shader::checkCompilationStatus(GLuint shaderID) {
	GLint status = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	
	if (status) {
		spdlog::info("Shader compilation was successful.");
	} else {
		char bufferLog[512];
		glGetShaderInfoLog(shaderID,512,nullptr,bufferLog);
		spdlog::error("Error compiling shader: {}",bufferLog);
	}
}

GLuint Shader::compileShader(const std::string& source, GLenum shader_type) {
	GLuint shader_id = glCreateShader(shader_type);	
	const char* src = source.c_str();
	spdlog::info("Shader compilation started, with contents: {}",src);
	
	glShaderSource(shader_id,1,&src,nullptr);
	glCompileShader(shader_id);
	checkCompilationStatus(shader_id);
	
	return shader_id;
}

void Shader::setVec3(std::string_view name, glm::vec3 &value) {
	GLint location = glGetUniformLocation(programID, name.data());
	if (location != -1) {
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
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

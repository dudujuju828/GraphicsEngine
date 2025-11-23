
#ifndef SRC_SHADER_HPP
#define SRC_SHADER_HPP

#include <string_view>
#include <string>
#include <filesystem>
#include <cstddef>
#include "../include/glad/glad.h"

class Shader {
private:
	GLuint programID;
	std::string loadFromFile(const std::filesystem::path path);
	GLuint compileShader(const std::string& source, GLenum shader_type);
	void createProgram(GLuint vertex_id, GLuint fragment_id);
	
public:
	Shader(std::filesystem::path &vertex_path, std::filesystem::path &fragment_path);

};

#endif

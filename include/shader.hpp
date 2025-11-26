
#ifndef SRC_SHADER_HPP
#define SRC_SHADER_HPP

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
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
	void checkCompilationStatus(GLuint shaderID);
	void createProgram(GLuint vertex_id, GLuint fragment_id);
	
public:
	Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path);
	GLuint getID();
	void useProgram();
	void setVec3(std::string_view name, glm::vec3 value);
	void setMat4(std::string_view name, glm::mat4 value);
	void setFloat(std::string_view name, float value);
	

};

#endif


#ifndef SRC_OBJECT_HPP
#define SRC_OBJECT_HPP

#include "../include/mesh.hpp"
#include "../include/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

struct Transform {
	glm::vec3 position{0.0f};
	glm::vec3 rotation{0.0f};
	glm::vec3 scale{1.0f};
	glm::mat4 modelMatrix{1.0f};
};

class Object {
	public:
	Object(std::filesystem::path mesh_path);
	Mesh mesh;
	Transform transform;
	
	void draw(Shader& shader);
	void setScale(glm::vec3 scale);
};

#endif

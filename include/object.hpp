
#ifndef SRC_OBJECT_HPP
#define SRC_OBJECT_HPP

#include "../include/mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 modelMatrix;
};

class Object {
	public:
	Object(std::filesystem::path mesh_path);
	Mesh mesh;
	Transform transform;
	
	void draw();
};

#endif

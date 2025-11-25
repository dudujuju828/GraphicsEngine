
#include "../include/object.hpp"
#include "../include/shader.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

Object::Object(std::filesystem::path mesh_path) : mesh(mesh_path) {
	spdlog::info("Created mesh object from obj at {}.",mesh_path.string());
}

void Object::draw(Shader& shader) {
	shader.setVec3("scale",this->transform.scale);
	mesh.draw();
}

void Object::setScale(glm::vec3 scale) {
	transform.scale = scale;
}

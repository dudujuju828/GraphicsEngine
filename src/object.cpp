
#include "../include/object.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

Object::Object(std::filesystem::path mesh_path) : mesh(mesh_path) {
	spdlog::info("Created mesh object from obj at {}.",mesh_path.string());
}

void Object::draw() {
	mesh.draw();
}


#include "../include/object.hpp"
#include "../include/shader.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Object::Object(std::filesystem::path mesh_path) : mesh(mesh_path) {
	spdlog::info("Created mesh object from obj at {}.",mesh_path.string());
}

void Object::draw(Shader& shader) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);

    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, transform.scale);

    shader.setMat4("model", model);
    mesh.draw();
}


void Object::setScale(glm::vec3 scale) {
	transform.scale = scale;
}

#ifndef SRC_SKYBOX_HPP
#define SRC_SKYBOX_HPP

#include "../include/glad/glad.h"
#include <array>
#include <filesystem>

#include <glm/glm.hpp>

class Shader;

class Skybox {
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint cubemap = 0;

    static GLuint loadCubemap(const std::array<std::filesystem::path, 6>& faces);
    void initCube();

public:
    Skybox() = delete;
    explicit Skybox(const std::array<std::filesystem::path, 6>& faces);
    ~Skybox();

    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;

    Skybox(Skybox&& other) noexcept;
    Skybox& operator=(Skybox&& other) noexcept;

    void draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
};

#endif

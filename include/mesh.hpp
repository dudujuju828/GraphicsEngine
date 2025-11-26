#ifndef SRC_MESH_HPP
#define SRC_MESH_HPP

#include "../include/glad/glad.h"
#include <filesystem>
#include <vector>

class Mesh {
private:
    GLuint VBO = 0;
    GLuint VAO = 0;
    int vertexCount = 0;

    void initFromPositions(const std::vector<float>& positions);

public:
    Mesh(const std::filesystem::path& objfile_path);
    Mesh(const std::vector<float>& positions);

    void use();
    void draw();
};

#endif


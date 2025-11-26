#ifndef SRC_MESH_HPP
#define SRC_MESH_HPP

#include "../include/glad/glad.h"
#include <filesystem>
#include <vector>

class Mesh {
private:
    GLuint VBO = 0;
    GLuint NBO = 0;         
    GLuint VAO = 0;
    int vertexCount = 0;
    bool hasNormals = false; 

    void initBuffers(const std::vector<float>& positions,
                     const std::vector<float>* normals);

public:
    Mesh(const std::filesystem::path& objfile_path);

    Mesh(const std::vector<float>& positions);

    void use();
    void draw();
};

#endif


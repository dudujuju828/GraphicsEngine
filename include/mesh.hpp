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
    ~Mesh();

    // constructors you already have
    Mesh(const std::filesystem::path& objfile_path);
    Mesh(const std::vector<float>& positions);
    Mesh(const std::vector<float>& positions,
         const std::vector<float>& normals);

    // NEW: disable copying (avoid double-delete / shallow copies)
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // NEW: enable moving (transfer ownership of VAO/VBO/NBO)
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void use();
    void draw();
};

#endif

#ifndef SRC_MESH_HPP
#define SRC_MESH_HPP

#include "../include/glad/glad.h"
#include <filesystem>
#include <vector>

struct aiScene;

class Mesh {
private:
    GLuint VBO = 0;
    GLuint NBO = 0;
    GLuint TBO = 0;
    GLuint VAO = 0;
    GLuint diffuseTexture = 0;

    int vertexCount = 0;
    bool hasNormals = false;
    bool hasTexCoords = false;

    void initBuffers(const std::vector<float>& positions,
                     const std::vector<float>* normals);
    void initBuffers(const std::vector<float>& positions,
                     const std::vector<float>* normals,
                     const std::vector<float>* texCoords);

    static GLuint loadTexture2D(const std::filesystem::path& path);
    static std::filesystem::path findFirstDiffuseTexture(const aiScene* scene,
                                                         const std::filesystem::path& directory);

public:
    ~Mesh();

    Mesh(const std::filesystem::path& objfile_path);
    Mesh(const std::vector<float>& positions);
    Mesh(const std::vector<float>& positions,
         const std::vector<float>& normals);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void use();
    void draw();
};

#endif

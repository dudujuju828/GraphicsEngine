#include "../include/glad/glad.h"
#include "../include/mesh.hpp"

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <stdexcept>
#include <vector>

void Mesh::initFromPositions(const std::vector<float>& positions) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(positions.size() * sizeof(float)),
        positions.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    vertexCount = static_cast<int>(positions.size() / 3);
}

Mesh::Mesh(const std::vector<float>& positions) {
    initFromPositions(positions);
}

Mesh::Mesh(const std::filesystem::path& objfile_path) {
    spdlog::info("Loading mesh with Assimp from {}", objfile_path.string());

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        objfile_path.string(),
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals      |
        aiProcess_ImproveCacheLocality  |
        aiProcess_SortByPType
    );

    if (!scene || !scene->HasMeshes()) {
        spdlog::error(
            "Assimp failed to load {}: {}",
            objfile_path.string(),
            importer.GetErrorString()
        );
        throw std::runtime_error(
            "Failed to load mesh with Assimp: " + objfile_path.string()
        );
    }

    const aiMesh* mesh = scene->mMeshes[0];

    if (!mesh->HasPositions()) {
        spdlog::error("Mesh {} has no vertex positions.", objfile_path.string());
        throw std::runtime_error(
            "Mesh has no vertex positions: " + objfile_path.string()
        );
    }

    std::vector<float> positions;
    positions.reserve(mesh->mNumFaces * 3u * 3u);

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            unsigned int idx = face.mIndices[j];
            const aiVector3D& v = mesh->mVertices[idx];

            positions.push_back(v.x);
            positions.push_back(v.y);
            positions.push_back(v.z);
        }
    }

    spdlog::info(
        "Loaded mesh {}: {} vertices ({} triangles).",
        objfile_path.string(),
        positions.size() / 3,
        positions.size() / 9
    );

    initFromPositions(positions);
}

void Mesh::use() {
    glBindVertexArray(VAO);
}

void Mesh::draw() {
    use();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


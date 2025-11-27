#include "../include/glad/glad.h"
#include "../include/mesh.hpp"

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <stdexcept>
#include <vector>

void Mesh::initBuffers(const std::vector<float>& positions,
                       const std::vector<float>* normals)
{
    hasNormals = (normals && !normals->empty());

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 positions.size() * sizeof(float),
                 positions.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (hasNormals) {
        glGenBuffers(1, &NBO);
        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBufferData(GL_ARRAY_BUFFER,
                     normals->size() * sizeof(float),
                     normals->data(),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    vertexCount = static_cast<int>(positions.size() / 3);
}

Mesh::Mesh(const std::vector<float>& positions)
{
    initBuffers(positions, nullptr);
}

Mesh::Mesh(const std::vector<float>& positions,
           const std::vector<float>& normals)
{
    initBuffers(positions, &normals);
}

Mesh::Mesh(const std::filesystem::path& objfile_path)
{
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
        spdlog::error("Assimp failed to load {}: {}",
                      objfile_path.string(), importer.GetErrorString());
        throw std::runtime_error("Failed to load mesh with Assimp: " + objfile_path.string());
    }

    const aiMesh* mesh = scene->mMeshes[0];

    if (!mesh->HasPositions()) {
        spdlog::error("Mesh {} has no vertex positions.", objfile_path.string());
        throw std::runtime_error("Mesh has no vertex positions: " + objfile_path.string());
    }

    std::vector<float> positions;
    std::vector<float> normals;
    positions.reserve(mesh->mNumFaces * 3u * 3u);
    normals.reserve(mesh->mNumFaces * 3u * 3u);

    bool hasAssimpNormals = mesh->HasNormals();

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];

        if (face.mNumIndices != 3) continue;

        for (unsigned int j = 0; j < 3; ++j) {
            unsigned int idx = face.mIndices[j];

            const aiVector3D& v = mesh->mVertices[idx];
            positions.push_back(v.x);
            positions.push_back(v.y);
            positions.push_back(v.z);

            if (hasAssimpNormals) {
                const aiVector3D& n = mesh->mNormals[idx];
                normals.push_back(n.x);
                normals.push_back(n.y);
                normals.push_back(n.z);
            }
        }

    }

    spdlog::info("Loaded mesh {}: {} vertices.",
                 objfile_path.string(), positions.size() / 3);

    std::vector<float>* normalsPtr = hasAssimpNormals ? &normals : nullptr;
    initBuffers(positions, normalsPtr);
}

void Mesh::use()
{
    glBindVertexArray(VAO);
}

void Mesh::draw()
{
    use();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


#include "../include/glad/glad.h"
#include "../include/mesh.hpp"

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <stdexcept>
#include <vector>

#include "../include/stb_image.h"

void Mesh::initBuffers(const std::vector<float>& positions,
                       const std::vector<float>* normals)
{
    initBuffers(positions, normals, nullptr);
}

void Mesh::initBuffers(const std::vector<float>& positions,
                       const std::vector<float>* normals,
                       const std::vector<float>* texCoords)
{
    hasNormals = (normals && !normals->empty());
    hasTexCoords = (texCoords && !texCoords->empty());

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

    if (hasTexCoords) {
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER,
                     texCoords->size() * sizeof(float),
                     texCoords->data(),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = static_cast<int>(positions.size() / 3);
}

Mesh::Mesh(const std::vector<float>& positions)
{
    initBuffers(positions, nullptr, nullptr);
}

Mesh::Mesh(const std::vector<float>& positions,
           const std::vector<float>& normals)
{
    initBuffers(positions, &normals, nullptr);
}

Mesh::Mesh(Mesh&& other) noexcept
    : VBO(other.VBO)
    , NBO(other.NBO)
    , TBO(other.TBO)
    , VAO(other.VAO)
    , diffuseTexture(other.diffuseTexture)
    , vertexCount(other.vertexCount)
    , hasNormals(other.hasNormals)
    , hasTexCoords(other.hasTexCoords)
{
    other.VBO = 0;
    other.NBO = 0;
    other.TBO = 0;
    other.VAO = 0;
    other.diffuseTexture = 0;
    other.vertexCount = 0;
    other.hasNormals = false;
    other.hasTexCoords = false;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (NBO) glDeleteBuffers(1, &NBO);
        if (TBO) glDeleteBuffers(1, &TBO);
        if (diffuseTexture) glDeleteTextures(1, &diffuseTexture);

        VBO = other.VBO;
        NBO = other.NBO;
        TBO = other.TBO;
        VAO = other.VAO;
        diffuseTexture = other.diffuseTexture;
        vertexCount = other.vertexCount;
        hasNormals = other.hasNormals;
        hasTexCoords = other.hasTexCoords;

        other.VBO = 0;
        other.NBO = 0;
        other.TBO = 0;
        other.VAO = 0;
        other.diffuseTexture = 0;
        other.vertexCount = 0;
        other.hasNormals = false;
        other.hasTexCoords = false;
    }
    return *this;
}

GLuint Mesh::loadTexture2D(const std::filesystem::path& path)
{
    if (path.empty() || !std::filesystem::exists(path)) {
        return 0;
    }

    int w = 0, h = 0, c = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &c, 0);
    if (!data) {
        return 0;
    }

    GLenum format = GL_RGB;
    if (c == 1) format = GL_RED;
    else if (c == 2) format = GL_RG;
    else if (c == 3) format = GL_RGB;
    else if (c == 4) format = GL_RGBA;

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

static std::filesystem::path resolveMaterialTexture(const aiMaterial* mat,
                                                    aiTextureType type,
                                                    const std::filesystem::path& directory)
{
    if (!mat || mat->GetTextureCount(type) == 0) return {};

    aiString str;
    if (mat->GetTexture(type, 0, &str) != AI_SUCCESS) return {};

    std::filesystem::path p = str.C_Str();
    if (p.is_absolute()) {
        if (std::filesystem::exists(p)) return p;
        return {};
    }

    std::filesystem::path full = directory / p;
    if (std::filesystem::exists(full)) return full;

    return {};
}

std::filesystem::path Mesh::findFirstDiffuseTexture(const aiScene* scene,
                                                    const std::filesystem::path& directory)
{
    if (!scene) return {};

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        const aiMaterial* mat = scene->mMaterials[i];

        std::filesystem::path p;

        p = resolveMaterialTexture(mat, aiTextureType_BASE_COLOR, directory);
        if (!p.empty()) return p;

        p = resolveMaterialTexture(mat, aiTextureType_DIFFUSE, directory);
        if (!p.empty()) return p;
    }

    return {};
}

Mesh::Mesh(const std::filesystem::path& objfile_path)
{
    spdlog::info("Loading mesh with Assimp from {}", objfile_path.string());

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        objfile_path.string(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType
    );

    if (!scene || !scene->HasMeshes()) {
        spdlog::error("Assimp failed to load {}: {}",
                      objfile_path.string(), importer.GetErrorString());
        throw std::runtime_error("Failed to load model: " + objfile_path.string());
    }

    bool useNormals = false;
    bool useTex = false;
    unsigned int totalTriangles = 0;

    for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
        const aiMesh* mesh = scene->mMeshes[m];
        if (!mesh || !mesh->HasPositions()) continue;

        if (mesh->HasNormals()) useNormals = true;
        if (mesh->HasTextureCoords(0)) useTex = true;

        for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace& face = mesh->mFaces[f];
            if (face.mNumIndices == 3) totalTriangles++;
        }
    }

    if (totalTriangles == 0) {
        throw std::runtime_error("Model contains no triangles: " + objfile_path.string());
    }

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> texCoords;

    positions.reserve(totalTriangles * 9u);
    if (useNormals) normals.reserve(totalTriangles * 9u);
    if (useTex) texCoords.reserve(totalTriangles * 6u);

    for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
        const aiMesh* mesh = scene->mMeshes[m];
        if (!mesh || !mesh->HasPositions()) continue;

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            const aiFace& face = mesh->mFaces[i];
            if (face.mNumIndices != 3) continue;

            for (unsigned int j = 0; j < 3; ++j) {
                unsigned int idx = face.mIndices[j];

                const aiVector3D& v = mesh->mVertices[idx];
                positions.push_back(v.x);
                positions.push_back(v.y);
                positions.push_back(v.z);

                if (useNormals) {
                    const aiVector3D& n = mesh->mNormals[idx];
                    normals.push_back(n.x);
                    normals.push_back(n.y);
                    normals.push_back(n.z);
                }

                if (useTex) {
                    if (mesh->HasTextureCoords(0)) {
                        const aiVector3D& t = mesh->mTextureCoords[0][idx];
                        texCoords.push_back(t.x);
                        texCoords.push_back(t.y);
                    } else {
                        texCoords.push_back(0.0f);
                        texCoords.push_back(0.0f);
                    }
                }
            }
        }
    }

    std::vector<float>* normalsPtr = useNormals ? &normals : nullptr;
    std::vector<float>* texPtr = useTex ? &texCoords : nullptr;

    initBuffers(positions, normalsPtr, texPtr);

    std::filesystem::path directory = objfile_path.parent_path();
    std::filesystem::path texPath = findFirstDiffuseTexture(scene, directory);
    if (!texPath.empty()) {
        diffuseTexture = loadTexture2D(texPath);
    }
}

Mesh::~Mesh()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (NBO) glDeleteBuffers(1, &NBO);
    if (TBO) glDeleteBuffers(1, &TBO);
    if (diffuseTexture) glDeleteTextures(1, &diffuseTexture);
}

void Mesh::use()
{
    glBindVertexArray(VAO);
}

void Mesh::draw()
{
    GLint program = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    if (program != 0) {
        GLint locUse = glGetUniformLocation(program, "uUseTexture");
        if (locUse != -1) glUniform1i(locUse, diffuseTexture ? 1 : 0);

        if (diffuseTexture) {
            GLint locMap = glGetUniformLocation(program, "uDiffuseMap");
            if (locMap != -1) glUniform1i(locMap, 0);
        }
    }

    if (diffuseTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    }

    use();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    if (diffuseTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

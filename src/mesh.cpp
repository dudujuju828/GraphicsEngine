#include "../include/glad/glad.h"
#include "../include/mesh.hpp"

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <stdexcept>
#include <vector>

/*
   * Initiailize buffers from vectors containing positions and normals,
   * is to be used internally in constructors. VAO and VBO handles stored
   * as internal members. Vertex count is also stored as needed for drawing
   * the mesh.
*/
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

/*
    * Constructors for when positions of a mesh and (if present) normals
    * of the object are available.
*/

Mesh::Mesh(const std::vector<float>& positions)
{
    initBuffers(positions, nullptr);
}

Mesh::Mesh(const std::vector<float>& positions,
           const std::vector<float>& normals)
{
    initBuffers(positions, &normals);
}

Mesh::Mesh(Mesh&& other) noexcept
    : VBO(other.VBO)
    , NBO(other.NBO)
    , VAO(other.VAO)
    , vertexCount(other.vertexCount)
    , hasNormals(other.hasNormals)
{
    other.VBO = 0;
    other.NBO = 0;
    other.VAO = 0;
    other.vertexCount = 0;
    other.hasNormals = false;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (NBO) glDeleteBuffers(1, &NBO);

        VBO = other.VBO;
        NBO = other.NBO;
        VAO = other.VAO;
        vertexCount = other.vertexCount;
        hasNormals = other.hasNormals;

        other.VBO = 0;
        other.NBO = 0;
        other.VAO = 0;
        other.vertexCount = 0;
        other.hasNormals = false;
    }
    return *this;
}

/*
    * Read in an object from path object. C++ 17 needed.
    * Triangulate converts all faces to triangles.
    * JoinIdenticalVertices merges any duplicates for efficiency.
    * GenSmoothNormals generates per-vertex normals if they are missing.
        * Also,
        * Computes smooth shading
    * ImproveCacheLocality, reorders the order of faces and vertex/index buffers
      to improve cache locality. Does not change geometry, uses cache-optimisation heuristic.
    * SortByPType splits and sorts meshes by primitive type, so aiMesh will only contain primitives
      of one type. As its combined with triangulate, will be triangles.
*/
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

    // First pass: count triangles and decide what to do with normals.
    std::size_t totalTriangles = 0;

    bool anyMeshHasNormals      = false;
    bool anyMeshMissingNormals  = false;

    for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
        const aiMesh* mesh = scene->mMeshes[m];

        if (!mesh->HasPositions()) {
            spdlog::warn("Sub-mesh {} in {} has no vertex positions; skipping.",
                         m, objfile_path.string());
            continue;
        }

        // We only care about triangle meshes.
        unsigned int meshTriangles = 0;
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            const aiFace& face = mesh->mFaces[i];
            if (face.mNumIndices == 3) {
                ++meshTriangles;
            }
        }

        if (meshTriangles == 0) {
            spdlog::warn("Sub-mesh {} in {} has no triangle faces; skipping.",
                         m, objfile_path.string());
            continue;
        }

        totalTriangles += meshTriangles;

        if (mesh->HasNormals())
            anyMeshHasNormals = true;
        else
            anyMeshMissingNormals = true;
    }

    if (totalTriangles == 0) {
        spdlog::error("No triangle geometry found in scene for {}", objfile_path.string());
        throw std::runtime_error("Scene has no triangle geometry: " + objfile_path.string());
    }

    // Consistent rule: either we use normals for all vertices, or we drop them.
    bool useNormals = anyMeshHasNormals && !anyMeshMissingNormals;
    if (!useNormals && anyMeshHasNormals) {
        spdlog::warn("Some sub-meshes in {} have normals and some do not; "
                     "dropping normals for consistency.",
                     objfile_path.string());
    }

    std::vector<float> positions;
    std::vector<float> normals;

    positions.reserve(totalTriangles * 3u * 3u); // triangles * 3 verts * 3 components
    if (useNormals) {
        normals.reserve(totalTriangles * 3u * 3u);
    }

    // Second pass: actually copy all triangle vertices (and normals, if used).
    for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
        const aiMesh* mesh = scene->mMeshes[m];

        if (!mesh->HasPositions())
            continue; // already warned above

        // Skip non-triangle-only meshes if they somehow slipped through.
        if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)) {
            spdlog::warn("Skipping non-triangle sub-mesh {} in {}", m, objfile_path.string());
            continue;
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            const aiFace& face = mesh->mFaces[i];

            if (face.mNumIndices != 3)
                continue; // should not happen after aiProcess_Triangulate, but be safe

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
            }
        }
    }

    spdlog::info("Loaded mesh {}: {} vertices from {} sub-meshes.",
                 objfile_path.string(), positions.size() / 3, scene->mNumMeshes);

    std::vector<float>* normalsPtr = useNormals ? &normals : nullptr;
    initBuffers(positions, normalsPtr);
}


Mesh::~Mesh() {
    if (VAO) glDeleteVertexArrays(1,&VAO);
    if (VBO) glDeleteBuffers(1,&VBO);
    if (NBO) glDeleteBuffers(1,&NBO);
}

/*
    * Bind the vertex array to the current context. Called internally
      when drawing.
*/
void Mesh::use()
{
    glBindVertexArray(VAO);
}

/*
    * Draw the mesh. Currently only draw arrays is supported which is inefficient.
    * Need to add indexed draws.
*/
void Mesh::draw()
{
    use();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


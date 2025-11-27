#include "../include/terrain.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace {

inline void pushVertex(std::vector<float>& v, float x, float y, float z) {
    v.push_back(x);
    v.push_back(y);
    v.push_back(z);
}

} // namespace

Mesh createTerrainMesh(
    unsigned int xSegments,
    unsigned int zSegments,
    float sizeX,
    float sizeZ,
    const siv::PerlinNoise& perlin,
    double frequency,
    int octaves,
    float amplitude
)
{
    std::vector<float> vertices;
    std::vector<float> normals;

    vertices.reserve(xSegments * zSegments * 6 * 3);
    normals.reserve(xSegments * zSegments * 6 * 3);

    const float halfX = sizeX * 0.5f;
    const float halfZ = sizeZ * 0.5f;

    auto sampleHeight = [&](float x, float z) -> float {
        double nx = static_cast<double>(x) * frequency;
        double nz = static_cast<double>(z) * frequency;
        double h = perlin.octave2D_01(nx, nz, octaves);
        h = h * 2.0 - 1.0;
        return static_cast<float>(h * amplitude);
    };

    for (unsigned int z = 0; z < zSegments; ++z) {
        float z0 = (static_cast<float>(z) / static_cast<float>(zSegments)) * sizeZ - halfZ;
        float z1 = (static_cast<float>(z + 1) / static_cast<float>(zSegments)) * sizeZ - halfZ;

        for (unsigned int x = 0; x < xSegments; ++x) {
            float x0 = (static_cast<float>(x) / static_cast<float>(xSegments)) * sizeX - halfX;
            float x1 = (static_cast<float>(x + 1) / static_cast<float>(xSegments)) * sizeX - halfX;

            float y00 = sampleHeight(x0, z0);
            float y10 = sampleHeight(x1, z0);
            float y01 = sampleHeight(x0, z1);
            float y11 = sampleHeight(x1, z1);

            glm::vec3 p0(x0, y00, z0);
            glm::vec3 p1(x1, y10, z0);
            glm::vec3 p2(x0, y01, z1);
            glm::vec3 p3(x1, y10, z0);
            glm::vec3 p4(x1, y11, z1);
            glm::vec3 p5(x0, y01, z1);

			glm::vec3 n0 = glm::normalize(glm::cross(p2 - p0, p1 - p0));
			glm::vec3 n1 = glm::normalize(glm::cross(p5 - p3, p4 - p3));
		 
            pushVertex(vertices, p0.x, p0.y, p0.z);
            pushVertex(vertices, p1.x, p1.y, p1.z);
            pushVertex(vertices, p2.x, p2.y, p2.z);
            for (int i = 0; i < 3; ++i) {
                normals.push_back(n0.x);
                normals.push_back(n0.y);
                normals.push_back(n0.z);
            }

            pushVertex(vertices, p3.x, p3.y, p3.z);
            pushVertex(vertices, p4.x, p4.y, p4.z);
            pushVertex(vertices, p5.x, p5.y, p5.z);
            for (int i = 0; i < 3; ++i) {
                normals.push_back(n1.x);
                normals.push_back(n1.y);
                normals.push_back(n1.z);
            }
        }
    }

    return Mesh(vertices, normals);
}


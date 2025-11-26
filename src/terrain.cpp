#include "../include/terrain.hpp"
#include <vector>

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
) {
    std::vector<float> vertices;
    vertices.reserve(xSegments * zSegments * 6 * 3); 

    auto heightAt = [&](unsigned int gx, unsigned int gz) -> float {
        double nx = gx * frequency;
        double nz = gz * frequency;

        // [0,1] octave Perlin
        double h = perlin.octave2D_01(nx, nz, octaves);
        // map to [-1,1]
        h = h * 2.0 - 1.0;
        return static_cast<float>(h * amplitude);
    };

    for (unsigned int z = 0; z < zSegments; ++z) {
        for (unsigned int x = 0; x < xSegments; ++x) {

            float u0 = static_cast<float>(x) / xSegments;
            float u1 = static_cast<float>(x + 1) / xSegments;
            float v0 = static_cast<float>(z) / zSegments;
            float v1 = static_cast<float>(z + 1) / zSegments;

            float x0 = (u0 - 0.5f) * sizeX;
            float x1 = (u1 - 0.5f) * sizeX;
            float z0 = (v0 - 0.5f) * sizeZ;
            float z1 = (v1 - 0.5f) * sizeZ;

            float y00 = heightAt(x,     z);
            float y10 = heightAt(x + 1, z);
            float y01 = heightAt(x,     z + 1);
            float y11 = heightAt(x + 1, z + 1);

            pushVertex(vertices, x0, y00, z0);
            pushVertex(vertices, x1, y10, z0);
            pushVertex(vertices, x0, y01, z1);

            pushVertex(vertices, x1, y10, z0);
            pushVertex(vertices, x1, y11, z1);
            pushVertex(vertices, x0, y01, z1);
        }
    }

    return Mesh(vertices);
}


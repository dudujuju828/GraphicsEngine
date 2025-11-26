
#ifndef SRC_TERRAIN_HPP
#define SRC_TERRAIN_HPP

#include "mesh.hpp"
#include "../external/PerlinNoise/PerlinNoise.hpp"

Mesh createTerrainMesh(
    unsigned int xSegments,
    unsigned int zSegments,
    float sizeX,
    float sizeZ,
    const siv::PerlinNoise& perlin,
    double frequency,
    int octaves,
    float amplitude
);

#endif

#include "../include/tree_field.hpp"
#include "../include/object.hpp"
#include "../include/shader.hpp"

#include <functional>

void TreeField::generate(int count,
                         glm::vec2 xRange,
                         glm::vec2 zRange,
                         const std::function<float(float, float)>& sampleHeight,
                         unsigned int seed)
{
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> xDist(xRange.x, xRange.y);
    std::uniform_real_distribution<float> zDist(zRange.x, zRange.y);
    std::uniform_real_distribution<float> sDist(0.25f, 0.55f);
    std::uniform_real_distribution<float> rDist(0.0f, 360.0f);

    positions.clear();
    scales.clear();
    rotY.clear();

    positions.reserve(count);
    scales.reserve(count);
    rotY.reserve(count);

    for (int i = 0; i < count; ++i) {
        float x = xDist(rng);
        float z = zDist(rng);
        float y = sampleHeight(x, z);

        positions.emplace_back(x, y, z);
        scales.push_back(sDist(rng));
        rotY.push_back(rDist(rng));
    }
}

void TreeField::draw(Object& treeObject, Shader& shader, int scaling_factor)
{
    for (std::size_t i = 0; i < positions.size(); ++i) {
        treeObject.transform.position = positions[i];
        treeObject.transform.rotation = glm::vec3(0.0f, rotY[i], 0.0f);
        treeObject.transform.scale = glm::vec3(scales[i] * scaling_factor);
        treeObject.draw(shader);
    }
}

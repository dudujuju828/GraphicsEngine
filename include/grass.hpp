#ifndef SRC_GRASS_HPP
#define SRC_GRASS_HPP

#include <glm/glm.hpp>
#include <functional>
#include <cstddef>

class Shader;

// Simple instanced grass field built from a quad.
class GrassField {
public:
    GrassField() = default;
    ~GrassField();

    // sampleHeight(x, z) should return the terrain height at (x, z)
    void init(
        std::size_t instanceCount,
        const glm::vec2& xRange,
        const glm::vec2& zRange,
        const std::function<float(float, float)>& sampleHeight);

    // grassTexture = OpenGL texture id
    void draw(Shader& shader,
              const glm::mat4& view,
              const glm::mat4& projection,
              float time,
              unsigned int grassTexture);

private:
    unsigned int vao_ = 0;
    unsigned int quadVbo_ = 0;
    unsigned int instancePosVbo_ = 0;
    unsigned int instanceScaleVbo_ = 0;
    unsigned int instanceRotVbo_ = 0;
    std::size_t instanceCount_ = 0;
};

#endif

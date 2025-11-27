#include "../include/glad/glad.h"
#include "../include/grass.hpp"
#include "../include/shader.hpp"

#include <glm/glm.hpp>
#include <random>
#include <vector>
#include <spdlog/spdlog.h>

GrassField::~GrassField() {
    if (instanceRotVbo_) glDeleteBuffers(1, &instanceRotVbo_);
    if (instanceScaleVbo_) glDeleteBuffers(1, &instanceScaleVbo_);
    if (instancePosVbo_) glDeleteBuffers(1, &instancePosVbo_);
    if (quadVbo_) glDeleteBuffers(1, &quadVbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

void GrassField::init(
    std::size_t instanceCount,
    const glm::vec2& xRange,
    const glm::vec2& zRange,
    const std::function<float(float, float)>& sampleHeight)
{
    instanceCount_ = static_cast<GLsizei>(instanceCount);

    float quad[] = {
        // pos.x, pos.y, pos.z,   u, v
        -0.5f, 0.0f, 0.0f,        0.0f, 0.0f,
         0.5f, 0.0f, 0.0f,        1.0f, 0.0f,
        -0.5f, 1.0f, 0.0f,        0.0f, 1.0f,
         0.5f, 1.0f, 0.0f,        1.0f, 1.0f
    };

    std::vector<glm::vec3> positions;
    std::vector<float> scales;
    std::vector<float> rotations;
    positions.reserve(instanceCount_);
    scales.reserve(instanceCount_);
    rotations.reserve(instanceCount_);

    std::mt19937 rng(1337u);
    std::uniform_real_distribution<float> distX(xRange.x, xRange.y);
    std::uniform_real_distribution<float> distZ(zRange.x, zRange.y);
    std::uniform_real_distribution<float> distScale(0.7f, 1.3f);
    std::uniform_real_distribution<float> distRot(0.0f, 6.2831853f);

    for (std::size_t i = 0; i < instanceCount_; ++i) {
        float x = distX(rng);
        float z = distZ(rng);
        float y = sampleHeight(x, z);

        positions.emplace_back(x, y, z);
        scales.push_back(distScale(rng));
        rotations.push_back(distRot(rng));
    }

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &quadVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glGenBuffers(1, &instancePosVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, instancePosVbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size() * sizeof(glm::vec3),
        positions.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    glVertexAttribDivisor(2, 1);

    glGenBuffers(1, &instanceScaleVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, instanceScaleVbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        scales.size() * sizeof(float),
        scales.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 1, GL_FLOAT, GL_FALSE,
        sizeof(float),
        (void*)0
    );
    glVertexAttribDivisor(3, 1);

    glGenBuffers(1, &instanceRotVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, instanceRotVbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        rotations.size() * sizeof(float),
        rotations.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4, 1, GL_FLOAT, GL_FALSE,
        sizeof(float),
        (void*)0
    );
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    spdlog::info("GrassField initialised with {} instances.", instanceCount_);
}

void GrassField::draw(Shader& shader,
                      const glm::mat4& view,
                      const glm::mat4& projection,
                      float time,
                      unsigned int grassTexture)
{
    if (!vao_ || instanceCount_ == 0) return;

    shader.useProgram();
    shader.setMat4("uView", view);
    shader.setMat4("uProj", projection);
    shader.setFloat("uTime", time);
    shader.setInt("uGrassTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);

    glBindVertexArray(vao_);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(instanceCount_));
    glBindVertexArray(0);
}


#ifndef SRC_TREE_FIELD_HPP
#define SRC_TREE_FIELD_HPP

#include <vector>
#include <random>
#include <glm/glm.hpp>

class Object;
class Shader;

class TreeField {
public:
    TreeField() = default;

    void generate(int count,
                  glm::vec2 xRange,
                  glm::vec2 zRange,
                  const std::function<float(float, float)>& sampleHeight,
                  unsigned int seed = 1337u);

    void draw(Object& treeObject, Shader& shader, int scaling_factor);

private:
    std::vector<glm::vec3> positions;
    std::vector<float> scales;
    std::vector<float> rotY;
};

#endif

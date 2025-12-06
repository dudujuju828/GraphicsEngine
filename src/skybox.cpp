#include "../include/skybox.hpp"
#include "../include/shader.hpp"
#include "../include/stb_image.h"

#include <glm/gtc/matrix_transform.hpp>

Skybox::Skybox(const std::array<std::filesystem::path, 6>& faces)
{
    initCube();
    cubemap = loadCubemap(faces);
}

Skybox::~Skybox()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (cubemap) glDeleteTextures(1, &cubemap);
}

Skybox::Skybox(Skybox&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), cubemap(other.cubemap)
{
    other.VAO = 0;
    other.VBO = 0;
    other.cubemap = 0;
}

Skybox& Skybox::operator=(Skybox&& other) noexcept
{
    if (this != &other) {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (cubemap) glDeleteTextures(1, &cubemap);

        VAO = other.VAO;
        VBO = other.VBO;
        cubemap = other.cubemap;

        other.VAO = 0;
        other.VBO = 0;
        other.cubemap = 0;
    }
    return *this;
}

void Skybox::initCube()
{
    float verts[] = {
        -1,  1, -1,  -1, -1, -1,   1, -1, -1,   1, -1, -1,   1,  1, -1,  -1,  1, -1,
        -1, -1,  1,  -1, -1, -1,  -1,  1, -1,  -1,  1, -1,  -1,  1,  1,  -1, -1,  1,
         1, -1, -1,   1, -1,  1,   1,  1,  1,   1,  1,  1,   1,  1, -1,   1, -1, -1,
        -1, -1,  1,  -1,  1,  1,   1,  1,  1,   1,  1,  1,   1, -1,  1,  -1, -1,  1,
        -1,  1, -1,   1,  1, -1,   1,  1,  1,   1,  1,  1,  -1,  1,  1,  -1,  1, -1,
        -1, -1, -1,  -1, -1,  1,   1, -1, -1,   1, -1, -1,  -1, -1,  1,   1, -1,  1
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

GLuint Skybox::loadCubemap(const std::array<std::filesystem::path, 6>& faces)
{
    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    stbi_set_flip_vertically_on_load(false);

    for (GLuint i = 0; i < 6; ++i) {
        int w = 0, h = 0, c = 0;
        unsigned char* data = stbi_load(faces[i].string().c_str(), &w, &h, &c, 0);
        if (!data) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            glDeleteTextures(1, &texID);
            return 0;
        }

        GLenum format = (c == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texID;
}

void Skybox::draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection)
{
    if (!cubemap || !VAO) return;

    glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(view));

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    shader.useProgram();
    shader.setMat4("view", viewNoTranslate);
    shader.setMat4("projection", projection);
    shader.setInt("skybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

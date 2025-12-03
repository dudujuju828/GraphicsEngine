#include <glad/glad.h>
#include "../include/texture_manager.hpp"
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

TextureManager::~TextureManager() {
    for (auto& [name, tex] : texture_mappings) {
        if (tex.ID != 0) {
            glDeleteTextures(1, &tex.ID);
        }
    }
}

void TextureManager::add(const std::string& path,
                         const std::string& name,
                         const TextureConfig& cfg)
{
    Texture tex{};
    glGenTextures(1, &tex.ID);
    tex.target = GL_TEXTURE_2D;

    glBindTexture(tex.target, tex.ID);

    glTexParameteri(tex.target, GL_TEXTURE_WRAP_S, cfg.wrapS);
    glTexParameteri(tex.target, GL_TEXTURE_WRAP_T, cfg.wrapT);
    glTexParameteri(tex.target, GL_TEXTURE_MIN_FILTER, cfg.minFilter);
    glTexParameteri(tex.target, GL_TEXTURE_MAG_FILTER, cfg.magFilter);

    stbi_set_flip_vertically_on_load(true); 

    int width = 0, height = 0, channels = 0;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data) {
        tex.width      = width;
        tex.height     = height;
        tex.nrChannels = channels;

        GLenum format = GL_RGB;
        switch (channels) {
            case 1: format = GL_RED;  break;
            case 2: format = GL_RG;   break;
            case 3: format = GL_RGB;  break;
            case 4: format = GL_RGBA; break;
            default: format = GL_RGBA; break;
        }

        glTexImage2D(tex.target, 0, format, width, height,
                     0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(tex.target);
    }

    stbi_image_free(data);
    glBindTexture(tex.target, 0);

    texture_mappings.insert_or_assign(name, tex);
}

void TextureManager::use(const std::string& name, GLuint unit) const {
    auto it = texture_mappings.find(name);
    if (it == texture_mappings.end()) return;

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(it->second.target, it->second.ID);
}

GLuint TextureManager::get(const std::string& name) const {
    auto it = texture_mappings.find(name);
    if (it == texture_mappings.end()) return 0;
    return it->second.ID;
}

void TextureManager::setParameters(const std::string& name,
                                   const TextureConfig& cfg)
{
    auto it = texture_mappings.find(name);
    if (it == texture_mappings.end()) return;

    const Texture& tex = it->second;
    glBindTexture(tex.target, tex.ID);

    glTexParameteri(tex.target, GL_TEXTURE_WRAP_S, cfg.wrapS);
    glTexParameteri(tex.target, GL_TEXTURE_WRAP_T, cfg.wrapT);
    glTexParameteri(tex.target, GL_TEXTURE_MIN_FILTER, cfg.minFilter);
    glTexParameteri(tex.target, GL_TEXTURE_MAG_FILTER, cfg.magFilter);

    glBindTexture(tex.target, 0);
}

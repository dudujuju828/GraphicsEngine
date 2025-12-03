
#include <glad/glad.h>
#include "../include/texture_manager.hpp"
#include <filesystem>
#include <string_view>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

void TextureManager::add(std::string path, std::string name) {
    Texture tex;
    glGenTextures(1,&tex.ID);
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    tex.target = GL_TEXTURE_2D;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char* c_path = path.c_str();
    unsigned char* data = stbi_load(c_path, &tex.width, &tex.height, &tex.nrChannels, 0);
	if (data) {
		GLenum format = (tex.nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
    
	stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    texture_mappings.insert({name,tex});
}

void TextureManager::use(const std::string& name, GLuint unit) {
    auto it = texture_mappings.find(name);
    if (it == texture_mappings.end()) return;

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(it->second.target, it->second.ID);
}

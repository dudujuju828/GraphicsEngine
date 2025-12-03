
#include <glad/glad.h>
#include "../include/texture_manager.hpp"
#include <filesystem>
#include <string_view>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

void TextureManager::add(std::filesystem::path path, std::string name) {
    Texture tex;
    glGenTextures(1,&tex.ID);
    glBindTexture(GL_TEXTURE_2D, tex.ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const char* c_path = path.string().c_str();
    unsigned char* data = stbi_load(c_path, &tex.width, &tex.height, &tex.nrChannels, 0);
	if (data) {
		GLenum format = (tex.nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
    
	stbi_image_free(data);
    texture_mappings.insert({name,tex});
}

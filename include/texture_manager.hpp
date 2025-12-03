#ifndef SRC_TEXTURE_MANAGER_HPP
#define SRC_TEXTURE_MANAGER_HPP

#include <glad/glad.h>
#include <unordered_map>
#include <string>


struct Texture {
    unsigned int ID = 0;
    GLenum       target = GL_TEXTURE_2D;
    int          width = 0;
    int          height = 0;
    int          nrChannels = 0;
};


struct TextureConfig {
    GLenum wrapS    = GL_REPEAT;
    GLenum wrapT    = GL_REPEAT;
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum magFilter = GL_LINEAR;
};

class TextureManager {
private:
    std::unordered_map<std::string, Texture> texture_mappings;

public:
    TextureManager() = default;
    ~TextureManager();                    

    void add(const std::string& path,
             const std::string& name,
             const TextureConfig& cfg = TextureConfig{});

    void use(const std::string& name, GLuint unit) const;
    GLuint get(const std::string& name) const;

    void setParameters(const std::string& name, const TextureConfig& cfg);
};

#endif

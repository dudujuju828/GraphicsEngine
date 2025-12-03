
#ifndef SRC_TEXTURE_MANAGER_HPP
#define SRC_TEXTURE_MANAGER_HPP

#include <glad/glad.h>
#include <filesystem>
#include <unordered_map>
#include <string>

struct Texture {
    unsigned int ID;
    GLenum target;
    int width,height,nrChannels;
};

class TextureManager {
    private:
    std::unordered_map<std::string, Texture> texture_mappings;

    public:
    TextureManager() = default;

    void add(std::filesystem::path texture_path, std::string name);


};

#endif 
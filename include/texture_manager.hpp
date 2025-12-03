
#ifndef SRC_TEXTURE_MANAGER_HPP
#define SRC_TEXTURE_MANAGER_HPP

#include <filesystem>
#include <unordered_map>
#include <string>

class TextureManager {
    private:
    std::unordered_map<std::string, int> texture_mappings;

    public:
    TextureManager();

    void add(std::filesystem::path texture_path);


};

#endif 
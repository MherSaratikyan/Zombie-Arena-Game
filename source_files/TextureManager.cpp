#include "../header_files/TextureManager.hpp"
#include <cassert>
#include <stdexcept>

TextureManager* TextureManager::m_instance = nullptr;

TextureManager::TextureManager(){
    assert(m_instance == nullptr);
    m_instance = this;
}

sf::Texture& TextureManager::get_texture(const std::string& filename){
    auto& m = m_instance->texture_map;

    auto key_value_pair = m.find(filename);

    if(key_value_pair != m.end()){
        return key_value_pair->second;
    }else{
        auto texture = m[filename];
        if(!texture.loadFromFile(filename)){
            throw std::runtime_error("Unable to load texture from \"filename\"");
        }
        return texture;
    }
}

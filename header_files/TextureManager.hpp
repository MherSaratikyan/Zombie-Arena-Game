#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager{
private:
    std::map<std::string,sf::Texture> texture_map;
    static TextureManager* m_instance;
public:
    TextureManager();
    static sf::Texture& get_texture(const std::string& filename);
};
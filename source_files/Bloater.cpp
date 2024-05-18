#include "../header_files/Bloater.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Bloater::spawn(double x, double y, int seed){
    m_texture = TextureManager::get_texture("../resource_files/graphics/bloater.png");
    m_sprite.setTexture(m_texture);
    m_health = bloater_health;
    std::srand(std::time(0) + seed);
    m_speed = bloater_speed + (bloater_speed * (10 - std::rand() % 20)) / 100;
    m_position.x = x;
    m_position.y = y;
    m_sprite.setOrigin(25,25);
    m_sprite.setPosition(m_position);
}
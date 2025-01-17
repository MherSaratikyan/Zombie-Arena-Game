#include "../header_files/Chaser.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Chaser::spawn(double x, double y, int seed){
    m_texture = TextureManager::get_texture("../resource_files/graphics/chaser.png");
    m_sprite.setTexture(m_texture);
    m_health = chaser_health;
    std::srand(std::time(0) + seed);
    m_speed = chaser_speed + (chaser_speed * (5 - std::rand() % 10)) / 100;
    m_position.x = x;
    m_position.y = y;
    m_sprite.setOrigin(25,25);
    m_sprite.setPosition(m_position);
}
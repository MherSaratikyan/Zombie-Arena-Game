#include "../header_files/Chaser.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Chaser::spawn(double x, double y, int seed){
    m_sprite.setTexture(TextureManager::get_texture("../resource_files/graphics/chaser.png"));
    m_health = chaser_health;
    std::srand(std::time(0) + seed);
    m_speed = chaser_speed + (chaser_speed * (5 - std::rand() % 10)) / 100;
}
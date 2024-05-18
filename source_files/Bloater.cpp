#include "../header_files/Bloater.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Bloater::spawn(double x, double y, int seed){
    m_sprite.setTexture(TextureManager::get_texture("../resource_files/graphics/bloater.png"));
    m_health = bloater_health;
    std::srand(std::time(0) + seed);
    m_speed = bloater_speed + (bloater_speed * (10 - std::rand() % 20)) / 100;
}
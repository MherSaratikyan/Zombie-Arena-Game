#include "../header_files/Praetorian.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Praetorian::spawn(double x, double y, int seed){
    m_sprite.setTexture(TextureManager::get_texture("../resource_files/graphics/chaser.png"));
    m_health = praetorian_health;
    std::srand(std::time(0) + seed);
    m_speed = praetorian_speed + (praetorian_speed * (2 - std::rand() % 4)) / 100;
}
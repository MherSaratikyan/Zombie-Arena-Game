#include "../header_files/Crawler.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Crawler::spawn(double x, double y, int seed){
    m_texture = TextureManager::get_texture("../resource_files/graphics/crawler.png");
    m_sprite.setTexture(m_texture);
    std::srand(std::time(0) + seed);
    m_speed = crawler_speed + (crawler_speed * (15 - std::rand() % 30)) / 100;
    m_health = crawler_health;
    m_position.x = x;
    m_position.y = y;
    m_sprite.setOrigin(25,25);
    m_sprite.setPosition(m_position);
}
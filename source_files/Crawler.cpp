#include "../header_files/Crawler.hpp"
#include "../header_files/TextureManager.hpp"
#include <cstdlib>
#include <ctime>

void Crawler::spawn(double x, double y, int seed){
    m_sprite.setTexture(TextureManager::get_texture("../resource_files/graphics/crawler.png"));
    std::srand(std::time(0) + seed);
    m_speed = crawler_speed + (crawler_speed * (15 - std::rand() % 30)) / 100;
    m_health = crawler_health;
}
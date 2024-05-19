#include "../header_files/Zombie.hpp"
#include "../header_files/TextureManager.hpp"

#include <cmath>

bool Zombie::hit(){
    --m_health;

    if(m_health == 0){
        m_alive = false;
        m_texture = TextureManager::get_texture("../resource_files/graphics/blood.png");
        m_sprite.setTexture(m_texture);
        return true;
    }

    return false;
}

bool Zombie::is_alive() const{
    return m_alive;
}

sf::FloatRect Zombie::get_position() const{
    return m_sprite.getGlobalBounds();
}

sf::Sprite Zombie::get_sprite() const{
    return m_sprite;
}

void Zombie::update(double elapsed_time, sf::Vector2f player_position){
    double player_x = player_position.x;
    double player_y = player_position.y;

    if(player_x > m_position.x){
        m_position.x += elapsed_time * m_speed;
    }

    if(player_x < m_position.x){
        m_position.x -= elapsed_time * m_speed;
    }

    if(player_y > m_position.y){
        m_position.y += elapsed_time * m_speed;
    }

    if(player_y < m_position.y){
        m_position.y -= elapsed_time * m_speed;
    }

    m_sprite.setPosition(m_position);

    double angle = std::atan2(player_y - m_position.y,player_x - m_position.x) * 180 / 3.1415926;

    m_sprite.setRotation(angle);
}


#include "../header_files/Player.hpp"
#include "../header_files/TextureManager.hpp"
#include <cmath>
#include <stdexcept>

Player::Player():
m_current_health{start_health},
m_max_health{start_health},
m_current_speed{start_speed}
{
    if(!m_texture.loadFromFile("../resource_files/graphics/player.png")){
        throw std::runtime_error("Error: Could not load the player texture");
    }

    m_sprite = sf::Sprite(TextureManager::get_texture("../resource_files/graphics/player.png"));
    m_sprite.setOrigin(25,25);
}

void Player::spawn(sf::IntRect arena,sf::Vector2f resolution, int tile_size){
    m_position.x = arena.width/2;
    m_position.y = arena.height/2;

    m_arena.left = arena.left;
    m_arena.width = arena.width;
    m_arena.top = arena.top;
    m_arena.height = arena.height;

    m_tile_size = tile_size;

    m_arena_resolution.x = resolution.x;
    m_arena_resolution.y = resolution.y;
}

void Player::reset_stats(){
    m_current_health = start_health;
    m_max_health = start_health;
    m_current_speed = start_speed;
}

bool Player::hit(sf::Time timeHit){
    if((timeHit.asMilliseconds() - m_last_hit.asMilliseconds()) > 200){
        m_last_hit = timeHit;
        m_current_health -= 10;
        return true;
    }

    return false;
}

sf::Time Player::get_last_hit_time() const{
    return m_last_hit;
}

sf::FloatRect Player::get_position() const{
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Player::get_center() const{
    return m_position;
}

double Player::get_rotation() const{
    return m_sprite.getRotation();
}

sf::Sprite Player::get_sprite() const{
    return m_sprite;
}

void Player::move_left(){
    m_moving_left = true;
}

void Player::move_right(){
    m_moving_right = true;
}

void Player::move_up(){
    m_moving_up = true;
}

void Player::move_down(){
    m_moving_down = true;
}

void Player::stop_left(){
    m_moving_left = false;
}

void Player::stop_right(){
    m_moving_right = false;
}

void Player::stop_up(){
    m_moving_up = false;
}

void Player::stop_down(){
    m_moving_down = false;
}

void Player::update(double elapsed_time, sf::Vector2i cursor_position){
    if(m_moving_up){
        m_position.y -= m_current_speed * elapsed_time;
        if(m_position.y < m_arena.top + m_tile_size){
            m_position.y = m_arena.top + m_tile_size;
        }
    }

    if(m_moving_down){
        m_position.y += m_current_speed * elapsed_time;
        if(m_position.y > m_arena.height - m_tile_size){
            m_position.y = m_arena.height - m_tile_size;
        }
    }

    if(m_moving_left){
        m_position.x -= m_current_speed * elapsed_time;
        if(m_position.x < m_arena.left + m_tile_size){
            m_position.x = m_arena.left + m_tile_size;
        }
    }

    if(m_moving_right){
        m_position.x += m_current_speed * elapsed_time;
        if(m_position.x > m_arena.width - m_tile_size){
            m_position.x = m_arena.width - m_tile_size;
        }
    }

    m_sprite.setPosition(m_position);

    double angle{std::atan2(cursor_position.y - m_arena_resolution.y/2,cursor_position.x - m_arena_resolution.x/2) * 180 / 3.1415926};
    m_sprite.setRotation(angle);

}

void Player::update_speed(){
    m_current_speed += start_speed * 0.2;
}

void Player::update_health(){
    m_max_health += start_health * 0.35;
}

void Player::increase_health_level(int amount){
    m_current_health += amount;
    if(m_current_health > m_max_health){
        m_current_health = m_max_health;
    }
}

int Player::get_health() const{
    return m_current_health;
}
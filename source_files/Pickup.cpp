#include "../header_files/Pickup.hpp"
#include "../header_files/TextureManager.hpp"

#include <cstdlib>
#include <ctime>

Pickup::Pickup(int type){
    m_type = type;

    if(m_type == 1){
        m_texture = TextureManager::get_texture("../resource_files/graphics/health_pickup.png");
        m_bonus = health_bonus;
    } else if(m_type == 2){
        m_texture = TextureManager::get_texture("../resource_files/graphics/ammo_pickup.png");
        m_bonus = ammo_bonus;
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(25,25);

    m_time_to_live = time_to_live;
    m_seconds_to_wait = wait_time;
}

void Pickup::set_arena(sf::IntRect arena){
    m_arena.left = arena.left + 50;
    m_arena.width = arena.width - 50;

    m_arena.top = arena.top + 50;
    m_arena.height = arena.height - 50;

    spawn();
}

void Pickup::spawn(){
    std::srand(time(0) / m_type);
    int x = std::rand() % m_arena.width;

    std::srand(time(0) * m_type);
    int y = std::rand() % m_arena.height;

    m_time_since_spawn = 0;
    m_spawned = true;

    m_sprite.setPosition(x,y);
}

sf::FloatRect Pickup::get_position() const{
    return m_sprite.getGlobalBounds();
}
    
sf::Sprite Pickup::get_sprite() const{
    return m_sprite;
}

void Pickup::update(double elapsed_time){
    if(m_spawned){
        m_time_since_spawn += elapsed_time;
    } else{
        m_time_since_despawn += elapsed_time;
    }

    if(m_time_since_spawn > m_time_to_live && m_spawned){
        m_spawned = false;
        m_time_since_despawn = 0;
    }

    if(m_time_since_despawn > m_seconds_to_wait && !m_spawned){
        spawn();
    }


}

bool Pickup::is_spawned() const{
    return m_spawned;
}

int Pickup::got_it(){
    m_spawned = false;
    m_time_since_despawn = 0;
    return m_bonus;
}

void Pickup::upgrade(){
    if (m_type == 1){
        m_bonus += (health_bonus * .5);
    }
    else{
        m_bonus += (ammo_bonus * .5);
    }
    // Make them more frequent and last longer
    m_time_to_live += (time_to_live / 10);
    m_seconds_to_wait -= (wait_time / 10);
}
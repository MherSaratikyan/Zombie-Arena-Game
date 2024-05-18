#pragma once

#include <SFML/Graphics.hpp>

class Pickup{
private:
    static constexpr int health_bonus{50};
    static constexpr int ammo_bonus{12};
    static constexpr int wait_time{10};
    static constexpr int time_to_live{5};

    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::IntRect m_arena;

    int m_type;
    int m_bonus;
    bool m_spawned;

    double m_time_since_spawn;
    double m_time_since_despawn;
    double m_time_to_live;
    double m_seconds_to_wait;

public:
    Pickup(int type);

    void set_arena(sf::IntRect arena);
    void spawn();

    sf::FloatRect get_position() const;
    sf::Sprite get_sprite() const;

    void update(double elapsed_time);

    bool is_spawned() const;

    int got_it();
    void upgrade();

};

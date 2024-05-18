#pragma once

#include <SFML/Graphics.hpp>

class Player{
private:
    const double start_speed{200};
    const double start_health{100};

private:
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    sf::Texture m_texture;


    sf::Vector2f m_arena_resolution;
    sf::IntRect m_arena;

    int m_tile_size;

    bool m_moving_up;
    bool m_moving_down;
    bool m_moving_right;
    bool m_moving_left;

private:
    double m_current_health;
    double m_max_health;
    sf::Time m_last_hit;
    double m_current_speed;

public:
    Player();

    void spawn(sf::IntRect arena,sf::Vector2f resolution, int tile_size);

    void reset_stats();

    bool hit(sf::Time timeHit);

    sf::Time get_last_hit_time() const;

    sf::FloatRect get_position() const;

    sf::Vector2f get_center() const;

    double get_rotation() const;

    sf::Sprite get_sprite() const;

    void move_left();

    void move_right();

    void move_up();

    void move_down();

    void stop_left();

    void stop_right();

    void stop_up();

    void stop_down();

    void update(double elapsed_time, sf::Vector2i cursor_position);

    void update_speed();

    void update_health();

    void increase_health_level(int amount);

    int get_health() const;
};
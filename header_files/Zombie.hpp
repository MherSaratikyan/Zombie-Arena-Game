#pragma once
#include <SFML/Graphics.hpp>

class Zombie{
protected:
    double m_speed;
    double m_health;

    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    sf::Texture m_texture;

    bool m_alive{true};

protected:
    static constexpr double bloater_speed{40};
    static constexpr double bloater_health{5};

    static constexpr double chaser_speed{80};
    static constexpr double chaser_health{1};

    static constexpr double crawler_speed{20};
    static constexpr double crawler_health{3};

 

public:
    bool hit();

    bool is_alive() const;

    virtual void spawn(double x, double y, int seed) = 0;

    sf::FloatRect get_position() const;

    sf::Sprite get_sprite() const;

    void update(double elapsed_time, sf::Vector2f player_location);

};
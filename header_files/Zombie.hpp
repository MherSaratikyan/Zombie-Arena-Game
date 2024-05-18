#pragma once
#include <SFML/Graphics.hpp>

class Zombie{
protected:
    double m_speed;
    double m_health;

    sf::Vector2f m_position;
    sf::Sprite m_sprite;

    bool m_alive;

protected:
    static const double bloater_speed{40};
    static const double bloater_health{5};

    static const double chaser_speed{80};
    static const double chaser_health{1};

    static const double crawler_speed{20};
    static const double crawler_health{3};

    static const double praetorian_speed{60};
    static const double praetorian_health{30};

public:
    bool hit();

    bool is_alive() const;

    virtual void spawn(double x, double y, int seed) = 0;

    sf::FloatRect get_position() const;

    sf::Sprite get_sprite() const;

    void update(double elapsed_time, sf::Vector2f player_location);

};
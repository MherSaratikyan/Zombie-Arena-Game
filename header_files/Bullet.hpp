#pragma once

#include <SFML/Graphics.hpp>

class Bullet{
private:
    sf::Vector2f m_position;

    sf::RectangleShape m_shape;

    bool m_in_flight{false};

    double m_speed{1000};
    double m_distance_x;
    double m_distance_y;

    double max_x;
    double max_y;

    double min_x;
    double min_y;

public:
    Bullet();

    void stop();

    bool is_in_flight() const;

    void shoot(double start_x, double start_y, double target_x, double target_y);

    sf::FloatRect get_position() const;
    sf::RectangleShape get_shape() const;

    void update(double elapsed_time);
};
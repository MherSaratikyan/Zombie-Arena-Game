#include "../header_files/Bullet.hpp"

Bullet::Bullet(){
    m_shape.setSize(sf::Vector2f(5,5));
}

void Bullet::stop(){
    m_in_flight = false;
}

bool Bullet::is_in_flight() const{
    return m_in_flight;
}

void Bullet::shoot(double start_x, double start_y, double target_x, double target_y){
    m_in_flight = true;
    m_position.x = start_x;
    m_position.y = start_y;

    double gradient = (start_y - target_y) / (start_x - target_x);

    if(gradient < 0){
        gradient *= -1;
    }

    double ratio_xy = m_speed / (1 + gradient);

    m_distance_x = ratio_xy;
    m_distance_y = ratio_xy * gradient;

    if(target_x < start_x){
        m_distance_x *= -1;
    }

    if(target_y < start_y){
        m_distance_y *= -1;
    }

    double range{1000};

    min_x = start_x - range;
    max_x = start_x + range;

    min_y = start_y - range;
    max_y = start_y + range;

    m_shape.setPosition(m_position);

}

sf::FloatRect Bullet::get_position() const{
    return m_shape.getGlobalBounds();
}

sf::RectangleShape Bullet::get_shape() const{
    return m_shape;
}

void Bullet::update(double elapsed_time){
    m_position.x += m_distance_x * elapsed_time;
    m_position.y += m_distance_y * elapsed_time;

    m_shape.setPosition(m_position);

    if(m_position.x < min_x || m_position.x > max_x || m_position.y < min_y || m_position.y > max_y){
        m_in_flight = false;
    }
}
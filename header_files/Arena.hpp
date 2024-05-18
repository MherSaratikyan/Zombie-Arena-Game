#pragma once
#include "../header_files/Zombie.hpp"
#include <SFML/Graphics.hpp>

int create_bacground(sf::VertexArray& rva, sf::IntRect arena);
std::vector<Zombie*> create_horde(int num_zombies, sf::IntRect arena);


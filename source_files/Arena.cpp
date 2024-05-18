#include "../header_files/Arena.hpp"
#include "../header_files/Zombie.hpp"
#include "../header_files/Crawler.hpp"
#include "../header_files/Chaser.hpp"
#include "../header_files/Bloater.hpp"

#include <vector>
#include <ctime>
#include <cstdlib>

int create_bacground(sf::VertexArray& rva, sf::IntRect arena){
    const int tile_size{50};
    const int tile_types{3};
    const int verts_in_quad{4};

    int world_width = arena.width / tile_size;
    int world_height = arena.height / tile_size;

    rva.setPrimitiveType(sf::Quads);
    rva.resize(world_height * world_width * verts_in_quad);
    
    int current_vertex{0};

    for(int w{0}; w < world_width; ++w){
        for(int h{0};h < world_height; ++h){
            rva[current_vertex].position = sf::Vector2f(w * tile_size,h * tile_size);
            rva[current_vertex + 1].position = sf::Vector2f((w + 1) * tile_size,h * tile_size);
            rva[current_vertex + 2].position = sf::Vector2f((w + 1) * tile_size,(h + 1) * tile_size);
            rva[current_vertex + 3].position = sf::Vector2f(w * tile_size,(h + 1) * tile_size);

            if(h == 0 || h == world_height - 1 || w == 0 || w == world_width - 1){
                //using wall texture
                rva[current_vertex].texCoords = sf::Vector2f(0,tile_types * tile_size);
                rva[current_vertex + 1].texCoords = sf::Vector2f(tile_size,tile_size * tile_types);
                rva[current_vertex + 2].texCoords = sf::Vector2f(tile_size, (tile_types + 1) * tile_size);
                rva[current_vertex + 3].texCoords = sf::Vector2f(0,(tile_size + 1) * tile_types);
            }else{
                //using random floor texture
                std::srand(std::time(0) + h * w - h);
                int mud_or_grass = std::rand() % tile_types;
                int vert_offset = mud_or_grass * tile_size;

                rva[current_vertex].texCoords = sf::Vector2f(0,vert_offset);
                rva[current_vertex + 1].texCoords = sf::Vector2f(tile_size, vert_offset);
                rva[current_vertex + 2].texCoords = sf::Vector2f(tile_size, tile_size + vert_offset);
                rva[current_vertex + 3].texCoords = sf::Vector2f(0,tile_size + vert_offset); 
            }

            current_vertex += verts_in_quad;
        }
    }

    return tile_size;
}

std::vector<Zombie*> create_horde(int num_zombies,sf::IntRect arena){
    std::vector<Zombie*> horde;
    horde.reserve(num_zombies);

    int max_y{arena.height - 20};
    int min_y{arena.top + 20};
    int max_x{arena.width - 20};
    int min_x{arena.left + 20};

    for(int i{0};i < num_zombies; ++i){
        std::srand(std::time(0) * i);
        int side = std::rand() % 4;

        double x;
        double y;

        switch(side){
            case 0:
                //left
                x = min_x;
                y = min_y + rand() % max_y;
                break;
            case 1:
                //right
                x = max_x;
                y = min_y + rand() % max_y;
                break;
            case 2:
                //top
                y = min_y;
                x = min_x + rand() % max_x;
                break;
            case 3:
                //bottom
                y = max_y;
                x = min_x + rand() % max_x;
                break;
        }

        //Bloater, Chaser or Crawler
        std::srand(std::time(0) * i * 2);
        int type = rand() % 3;

        switch(type){
            case 0:
                //Chaser
                horde.push_back(new Chaser);
                break;
            case 1:
                //Crawler
                horde.push_back(new Crawler);
                break;
            case 2:
                //Bloater
                horde.push_back(new Bloater);
                break; 
        }

        horde[i]->spawn(x,y,i);

    }

    return horde;

}
#include <SFML/Graphics.hpp>
#include "../header_files/Player.hpp"
#include "../header_files/Arena.hpp"

int main(){
    enum class State {PAUSED, LEVEL_UP, GAME_OVER, PLAYING};

    State curr_state = State::GAME_OVER;

    sf::Vector2f resolution;

    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y),"Zombie Arena",sf::Style::Fullscreen);

    sf::View main_view(sf::FloatRect(0,0,resolution.x,resolution.y));

    sf::Clock clock;
    sf::Time total_game_time;

    //cursor position relative to the world
    sf::Vector2f cursor_world_position;

    //cursor position relative to screen coordinates
    sf::Vector2i cursor_screen_position;

    Player player;

    sf::IntRect arena;

    sf::VertexArray background;
    sf::Texture background_texture;
    background_texture.loadFromFile("../resource_files/graphics/background_sheet.png");

    while(window.isOpen()){
        //handling input
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Return && curr_state == State::PLAYING){
                    curr_state = State::PAUSED;
                }
            } else if(event.key.code == sf::Keyboard::Return && curr_state == State::PAUSED){
                curr_state = State::PLAYING;
                clock.restart();
            } else if(event.key.code == sf::Keyboard::Return && curr_state == State::GAME_OVER){
                curr_state = State::LEVEL_UP;
            }else if(curr_state == State::PLAYING){
                //handle player pressing w/a/s/d  while playing
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                    player.move_up();
                } else {
                    player.stop_up();
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    player.move_down();
                } else {
                    player.stop_down();
                } 

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                    player.move_left();
                } else {
                    player.stop_left();
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                    player.move_right();
                } else {
                    player.stop_right();
                }
            }//end wasd while playing

            //handle leveling up
            if(curr_state == State::LEVEL_UP){
                switch(event.key.code){
                    case sf::Keyboard::Num1:
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num2:
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num3:
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num4:
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num5:
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num6:
                        curr_state = State::PLAYING;
                        break;
                }

                if(curr_state == State::PLAYING){
                    arena.width = 500;
                    arena.height = 500;
                    arena.left = 0;
                    arena.top = 0;

                    int tile_size{create_bacground(background,arena)};
                    //int tile_size = 50;

                    player.spawn(arena,resolution,tile_size);
                    clock.restart();
                }
            }
        }//end event polling

        //handle user quiting
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        }

        //update the frame
        if(curr_state == State::PLAYING){
            sf::Time dt = clock.restart();
            total_game_time += dt;

            double dt_as_seconds{dt.asSeconds()};

            cursor_screen_position = sf::Mouse::getPosition();
            cursor_world_position = window.mapPixelToCoords(
                sf::Mouse::getPosition(),main_view
            );

            player.update(dt_as_seconds,sf::Mouse::getPosition());

            sf::Vector2f player_position(player.get_center());

        }

        //draw the scene
        if(curr_state == State::PLAYING){
            window.clear();

            window.setView(main_view);

            window.draw(background,&background_texture);
            window.draw(player.get_sprite());
        }

        if(curr_state == State::LEVEL_UP){

        }

        if(curr_state == State::PAUSED){

        }

        if(curr_state == State::GAME_OVER){

        }

        window.display();
    }



    return 0;

}
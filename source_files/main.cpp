#include <SFML/Graphics.hpp>
#include <vector>

#include "../header_files/Player.hpp"
#include "../header_files/Arena.hpp"
#include "../header_files/TextureManager.hpp"
#include "../header_files/Bullet.hpp"
#include "../header_files/Pickup.hpp"


int main(){
    TextureManager manager;

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
    sf::Texture background_texture = TextureManager::get_texture("../resource_files/graphics/background_sheet.png");
    
    //Prepare the horde
    int num_zombies;
    int num_zombies_alive;
    std::vector<Zombie*> zombie_horde;

    //Prepare bullets
    std::vector<Bullet> bullets(100);
    int curr_bullet{0};
    int bullet_spare{24};
    int bullets_in_clip{6};
    int clip_size{6};
    double fire_rate{1};
    sf::Time last_pressed;

    window.setMouseCursorVisible(false);
    sf::Sprite crosshair_sprite;
    sf::Texture crosshair_texture = TextureManager::get_texture("../resource_files/graphics/crosshair.png");
    crosshair_sprite.setTexture(crosshair_texture);
    crosshair_sprite.setOrigin(25,25);

    //Creating pickups
    Pickup health_pickup(1);
    Pickup ammo_pickup(2);

    //variavles for score storing
    int curr_score{0};
    int hi_score{0};

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

                //Shooting
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    if(total_game_time.asMilliseconds() - last_pressed.asMilliseconds() > 1000/fire_rate && bullets_in_clip > 0){
                        bullets[curr_bullet].shoot(
                            player.get_center().x, player.get_center().y,
                            cursor_world_position.x, cursor_world_position.y);
                        
                        ++curr_bullet;
                        if(curr_bullet > 99){
                            curr_bullet = 0;
                        }

                        last_pressed = total_game_time;
                        --bullets_in_clip;
                    }
                }
            }

            

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

                    arena.width = 1000;
                    arena.height = 1000;
                    arena.left = 0;
                    arena.top = 0;

                    int tile_size{create_bacground(background,arena)};
                    //int tile_size = 50;

                    player.spawn(arena,resolution,tile_size);

                    health_pickup.set_arena(arena);
                    ammo_pickup.set_arena(arena);

                    num_zombies = 15;
                    for(int z{0}; z < zombie_horde.size();++z){
                        delete zombie_horde[z];
                    }

                    zombie_horde = create_horde(num_zombies,arena);
                    num_zombies_alive = num_zombies;

                    clock.restart();
                }
            }

            //Reloading
            if(event.key.code == sf::Keyboard::R){
                if(bullet_spare >= clip_size){
                    bullets_in_clip = clip_size;
                    bullet_spare -= clip_size;
                } else if(bullet_spare > 0){
                    bullets_in_clip = bullet_spare;
                    bullet_spare = 0;
                }else{

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

            crosshair_sprite.setPosition(cursor_world_position);

            player.update(dt_as_seconds,sf::Mouse::getPosition());

            sf::Vector2f player_position(player.get_center());
            main_view.setCenter(player.get_center());

            //loop in horde of zombies and update them
            for(int z{0}; z < zombie_horde.size();++z){
                if(zombie_horde[z]->is_alive()){
                    zombie_horde[z]->update(dt.asSeconds(),player_position);
                }
            }

            for(int b{0}; b < 100; ++b){
                if(bullets[b].is_in_flight()){
                    bullets[b].update(dt.asSeconds());
                }
            }

            health_pickup.update(dt.asSeconds());
            ammo_pickup.update(dt.asSeconds());

            //Collision detection
            //Zombie and bullet collision detection and processing
            for(int i{0}; i < 100; ++i){
                for(int j{0}; j < num_zombies; ++j){
                    if(bullets[i].is_in_flight() && zombie_horde[j]->is_alive()){
                        if(bullets[i].get_position().intersects(zombie_horde[j]->get_position())){
                            bullets[i].stop();
                            if(zombie_horde[j]->hit()){
                                curr_score += 10;
                                if(curr_score > hi_score){
                                    hi_score = curr_score;
                                }

                                --num_zombies_alive;

                                if(num_zombies_alive == 0){
                                    curr_state = State::LEVEL_UP;
                                }
                            }
                        }
                    }
                }
            }

            //Zombie and player collision detection and processing
            for(int i{0}; i < num_zombies;++i){
                if((player.get_position().intersects(zombie_horde[i]->get_position())) && zombie_horde[i]->is_alive()){
                    if(player.hit(total_game_time)){

                    }

                    if(player.get_health() <= 0){
                        curr_state = State::GAME_OVER;
                    }
                }
            }

            //Player and health pickup collision detection and processing
            if((player.get_position().intersects(health_pickup.get_position())) && health_pickup.is_spawned()){
                player.increase_health_level(health_pickup.got_it());
            }

            //Player and ammo pickup collision detection and processing
            if((player.get_position().intersects(ammo_pickup.get_position())) && ammo_pickup.is_spawned()){
                bullet_spare += ammo_pickup.got_it();
            }

        }

        

        //draw the scene
        if(curr_state == State::PLAYING){
            window.clear();

            window.setView(main_view);

            window.draw(background,&background_texture);

            for(int z{0};z < zombie_horde.size();++z){
                window.draw(zombie_horde[z]->get_sprite());
            }

            for(int b{0}; b < 100; ++b){
                if(bullets[b].is_in_flight()){
                    window.draw(bullets[b].get_shape());
                }
            }


            window.draw(player.get_sprite());

            if(health_pickup.is_spawned()){
                window.draw(health_pickup.get_sprite());
            }

            if(ammo_pickup.is_spawned()){
                window.draw(ammo_pickup.get_sprite());
            }

            window.draw(crosshair_sprite);
        }

        if(curr_state == State::LEVEL_UP){

        }

        if(curr_state == State::PAUSED){

        }

        if(curr_state == State::GAME_OVER){

        }

        window.display();
    }


    for(int z{0};z < zombie_horde.size();++z){
        delete zombie_horde[z];
    }

    return 0;

}
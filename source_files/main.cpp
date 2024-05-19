#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

    //Game over/home screen
    sf::Sprite game_over_sprite;
    sf::Texture game_over_texture = TextureManager::get_texture("../resource_files/graphics/background.png");
    game_over_sprite.setTexture(game_over_texture);
    game_over_sprite.setPosition(0,0);

    //View for HUD
    sf::View hud_view(sf::FloatRect(0,0,resolution.x,resolution.y));

    //Ammo icon sprite
    sf::Sprite ammo_icon_sprite;
    sf::Texture ammo_icon_texture = TextureManager::get_texture("../resource_files/graphics/ammo_icon.png");
    ammo_icon_sprite.setTexture(ammo_icon_texture);
    ammo_icon_sprite.setPosition(20,980);

    //Loading the font
    sf::Font game_font;
    game_font.loadFromFile("../resource_files/font/zombiecontrol.ttf");

    //Paused
    sf::Text paused_text;
    paused_text.setFont(game_font);
    paused_text.setCharacterSize(155);
    paused_text.setFillColor(sf::Color::White);
    paused_text.setPosition(400,400);
    paused_text.setString("Press Enter \nTo Continue");

    //Game over
    sf::Text game_over_text;
    game_over_text.setFont(game_font);
    game_over_text.setCharacterSize(125);
    game_over_text.setFillColor(sf::Color::White);
    game_over_text.setPosition(250,850);
    game_over_text.setString("Press Enter To Play");

    //Level Up
    sf::Text level_up_text;
    level_up_text.setFont(game_font);
    level_up_text.setCharacterSize(80);
    level_up_text.setFillColor(sf::Color::White);
    level_up_text.setPosition(150,250);
    std::stringstream level_up_string;
    level_up_string <<
                    "1 - Increased fire rate" <<
                    "\n2 - Increased clip size(next reload)" <<
                    "\n3 - Increased max health" <<
                    "\n4 - Increased run speed" <<
                    "\n5 - More and better health pickups" <<
                    "\n6 - More and better ammo pickups";
    level_up_text.setString(level_up_string.str());

    //Ammo
    sf::Text ammo_text;
    ammo_text.setFont(game_font);
    ammo_text.setCharacterSize(55);
    ammo_text.setFillColor(sf::Color::White);
    ammo_text.setPosition(200,980);

    //Score
    sf::Text score_text;
    score_text.setFont(game_font);
    score_text.setCharacterSize(55);
    score_text.setFillColor(sf::Color::White);
    score_text.setPosition(20, 0);

    std::ifstream read_from_file("../resource_files/gamedata/scores.txt");
    if(!read_from_file.is_open()){
        hi_score = 0;
    }else{
        read_from_file >> hi_score;
        read_from_file.close();
    }

    

    //High Score
    sf::Text hi_score_text;
    hi_score_text.setFont(game_font);
    hi_score_text.setCharacterSize(55);
    hi_score_text.setFillColor(sf::Color::White);
    std::stringstream s;
    s << "Hi score: " << hi_score;
    hi_score_text.setString(s.str());
    hi_score_text.setPosition(1400, 0);

    //Zombies remaining
    sf::Text remaining_zombie_text;
    remaining_zombie_text.setFont(game_font);
    remaining_zombie_text.setCharacterSize(55);
    remaining_zombie_text.setFillColor(sf::Color::White);
    remaining_zombie_text.setPosition(1500, 980);
    remaining_zombie_text.setString("Zombies: 100");

    //Wave number
    sf::Text wave_number_text;
    int wave = 0;
    wave_number_text.setFont(game_font);
    wave_number_text.setCharacterSize(55);
    wave_number_text.setFillColor(sf::Color::White);
    wave_number_text.setPosition(1250, 980);
    wave_number_text.setString("Wave: 0");

    //Health bar
    sf::RectangleShape health_bar;
    health_bar.setFillColor(sf::Color::Red);
    health_bar.setPosition(450,980);


    //debug HUD
    sf::Text debug_text;
    debug_text.setFont(game_font);
    debug_text.setCharacterSize(25);
    debug_text.setFillColor(sf::Color::White);
    debug_text.setPosition(20,220);
    std::ostringstream ss;

    int frame_since_last_hud_update{0};
    int fps_measurement_frame_interval{1000};

    //Prepare the hit sound
    sf::SoundBuffer hit_buffer;
    hit_buffer.loadFromFile("../resource_files/sound/hit.wav");
    sf::Sound hit_sound;
    hit_sound.setBuffer(hit_buffer);

    // Prepare the splat sound
    sf::SoundBuffer splat_buffer;
    splat_buffer.loadFromFile("../resource_files/sound/splat.wav");
    sf::Sound splat_sound;
    splat_sound.setBuffer(splat_buffer);

    // Prepare the shoot sound
    sf::SoundBuffer shoot_buffer;
    shoot_buffer.loadFromFile("../resource_files/sound/shoot.wav");
    sf::Sound shoot_sound;
    shoot_sound.setBuffer(shoot_buffer);

    // Prepare the reload sound
    sf::SoundBuffer reload_buffer;
    reload_buffer.loadFromFile("../resource_files/sound/reload.wav");
    sf::Sound reload_sound;
    reload_sound.setBuffer(reload_buffer);

    // Prepare the failed sound
    sf::SoundBuffer reload_failed_buffer;
    reload_failed_buffer.loadFromFile("../resource_files/sound/reload_failed.wav");
    sf::Sound reload_failed_sound;
    reload_failed_sound.setBuffer(reload_failed_buffer);

    // Prepare the powerup sound
    sf::SoundBuffer powerup_buffer;
    powerup_buffer.loadFromFile("../resource_files/sound/powerup.wav");
    sf::Sound powerup_sound;
    powerup_sound.setBuffer(powerup_buffer);

    // Prepare the pickup sound
    sf::SoundBuffer pickup_buffer;
    pickup_buffer.loadFromFile("../resource_files/sound/pickup.wav");
    sf::Sound pickup_sound;
    pickup_sound.setBuffer(pickup_buffer);

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

                wave = 0;
                curr_score = 0;

                curr_bullet = 0;
                bullet_spare = 24;

                bullets_in_clip = 6;
                clip_size = 6;

                fire_rate = 1;

                player.reset_stats();
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

                        shoot_sound.play();
                        --bullets_in_clip;
                    }
                }
            }

            

            //handle leveling up
            if(curr_state == State::LEVEL_UP){
                switch(event.key.code){
                    case sf::Keyboard::Num1:
                        //increase fire rate
                        ++fire_rate;
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num2:
                        //increase clip size
                        clip_size += clip_size;
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num3:
                        //increase health
                        player.update_health();
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num4:
                        //increase speed
                        player.update_speed();
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num5:
                        //upgrade health pickup
                        health_pickup.upgrade();
                        curr_state = State::PLAYING;
                        break;
                    case sf::Keyboard::Num6:
                        //upgrade ammo pickup
                        ammo_pickup.upgrade();
                        curr_state = State::PLAYING;
                        break;
                }

                if(curr_state == State::PLAYING){
                    
                    //increase wave number
                    ++wave;

                    arena.width = 1500;
                    arena.height = 1500;
                    arena.left = 0;
                    arena.top = 0;

                    int tile_size{create_bacground(background,arena)};
                    //int tile_size = 50;

                    player.spawn(arena,resolution,tile_size);

                    health_pickup.set_arena(arena);
                    ammo_pickup.set_arena(arena);

                    num_zombies = 10 * wave;
                    for(int z{0}; z < zombie_horde.size();++z){
                        delete zombie_horde[z];
                    }

                    powerup_sound.play();

                    zombie_horde = create_horde(num_zombies,arena);
                    num_zombies_alive = num_zombies;

                    clock.restart();
                }
            }

            //Reloading
            if(event.key.code == sf::Keyboard::R){
                int need_to_fill_clip = clip_size - bullets_in_clip;
                if(bullet_spare >= need_to_fill_clip){
                    bullets_in_clip += need_to_fill_clip;
                    bullet_spare -= need_to_fill_clip;
                    reload_sound.play();
                } else if(bullet_spare > 0){
                    bullets_in_clip += bullet_spare;
                    bullet_spare = 0;
                    reload_sound.play();
                }else{
                    reload_failed_sound.play();
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

                                splat_sound.play();
                            }
                        }
                    }
                }
            }

            //Zombie and player collision detection and processing
            for(int i{0}; i < num_zombies;++i){
                if((player.get_position().intersects(zombie_horde[i]->get_position())) && zombie_horde[i]->is_alive()){
                    if(player.hit(total_game_time)){
                        hit_sound.play();
                    }

                    if(player.get_health() <= 0){
                        curr_state = State::GAME_OVER;
                        std::ofstream write_in_file("../resource_files/gamedata/scores.txt");
                        write_in_file << hi_score;
                        write_in_file.close();
                    }
                }
            }

            //Player and health pickup collision detection and processing
            if((player.get_position().intersects(health_pickup.get_position())) && health_pickup.is_spawned()){
                player.increase_health_level(health_pickup.got_it());
                pickup_sound.play();
            }

            //Player and ammo pickup collision detection and processing
            if((player.get_position().intersects(ammo_pickup.get_position())) && ammo_pickup.is_spawned()){
                bullet_spare += ammo_pickup.got_it();
                reload_sound.play();
            }

            //sizing up health bar
            health_bar.setSize(sf::Vector2f(player.get_health() * 3, 50));

            ++frame_since_last_hud_update;

            if(frame_since_last_hud_update > fps_measurement_frame_interval){
                std::stringstream ss_ammo;
                std::stringstream ss_score;
                std::stringstream ss_hi_score;
                std::stringstream ss_wave;
                std::stringstream ss_zombies_alive;

                //Update ammo text
                ss_ammo << bullets_in_clip <<'/'<<bullet_spare;
                ammo_text.setString(ss_ammo.str());

                //Update score text
                ss_score << "Score: "<<curr_score;
                score_text.setString(ss_score.str());

                //Update High score text
                ss_hi_score << "High Score: "<<hi_score;
                hi_score_text.setString(ss_hi_score.str());

                //Update the wave
                ss_wave << "Wave: "<<wave;
                wave_number_text.setString(ss_wave.str());

                //Update remaining zombies text
                ss_zombies_alive << "Zombies: "<<num_zombies_alive;
                remaining_zombie_text.setString(ss_zombies_alive.str());

                frame_since_last_hud_update = 0;

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

            //switch to hud view
            window.setView(hud_view);

            window.draw(ammo_icon_sprite);
            window.draw(ammo_text);
            window.draw(score_text);
            window.draw(hi_score_text);
            window.draw(health_bar);
            window.draw(wave_number_text);
            window.draw(remaining_zombie_text);
        }

        if(curr_state == State::LEVEL_UP){
            window.draw(game_over_sprite);
            window.draw(level_up_text);
        }

        if(curr_state == State::PAUSED){
            window.draw(paused_text);
        }

        if(curr_state == State::GAME_OVER){
            window.draw(game_over_sprite);
            window.draw(game_over_text);
            window.draw(score_text);
            window.draw(hi_score_text);
        }

        window.display();
    }


    for(int z{0};z < zombie_horde.size();++z){
        delete zombie_horde[z];
    }

    return 0;

}
//#include "splashkit.h"
#include "game.h"
#include "player.h"

using namespace std;

// used to get M_PI and math functions cosine and sine
#define _USE_MATH_DEFINES
#include <math.h>

// start the level
void new_level(game_data &game) // add asteroids to game
{
    game.asteroids.clear(); // clear existing asteroids, only relevant when new game is started    
    
    // add asteroids to the games asteroid vector
    for(int i = 0; i < game.asteroid_count; i++)
    {
        game.asteroids.push_back(new_roid(rnd(screen_width()), rnd(screen_height()), 3, 1, game.player));
    }    
}

// create a new game
game_data new_game()
{
    game_data result;
    result.menu_active = true; // initially menu is displayed
    result.player = new_player();
    result.player.velocity.x = 0;
    result.player.velocity.y = 0;
    result.asteroid_count = 3;
    new_level(result);
    return result;
}

// draw all elements of the game to the screen
void draw_game(const game_data &game)
{
    // Clear the screen for redrawing
    clear_screen(COLOR_BLACK);

    // draw backdrop image
    draw_bitmap(bitmap_named("back"), 0, 0, option_to_screen());

    // draw the player
    draw_player(game.player);
    
    // if immunity is active show the halo layer
    if(timer_ticks(game.player.player_timer) > IMMUNITY_TIME){
        sprite_hide_layer(game.player.main_sprite, 2);
    }

    // draw the bullets
    for(int i = 0; i < game.bullets.size(); i++)
    { 
        draw_bullet(game.bullets[i]);
    }

    // draw asteroids 
    for(int i = 0; i < game.asteroids.size(); i++)
    { 
        draw_roid(game.asteroids[i]);
    }

    // draw hud
    draw_hud(game.player);

    //refresh the screen
    refresh_screen(60);
}

// update all game elements
void update_game(game_data &game)
{
     // Update the players location
    update_player(game.player); 

    //Update bullets, check bullet life, if expired delete sprite, timer and entry in vector.
    for(int i = 0; i < game.bullets.size(); i++)
    {          
        if(timer_ticks(game.bullets[i].bullet_timer) > BULLET_LIFE){ // bullet expired
            free_sprite(game.bullets[i].main_sprite); // remove sprite
            timer tmp_timer = timer_named(game.bullets[i].timer_name); // reference to bullets timer
            free_timer(tmp_timer); // delete timer
            game.bullets.erase(game.bullets.begin() + i); // remove from vector
        }
        else{        
            update_bullet(game.bullets[i]); // bullet still active update it
        }
    }

    // update roids
    for(int i = 0; i < game.asteroids.size(); i++)
    { 
        update_roid(game.asteroids[i]);
    }

    // check for collisions
    check_collisions(game);
}

// screen wrapping, check if sprite is off screen. if so, place it on other side.
void screen_wrap(point_2d &target_object, sprite s)
{    
    if(target_object.x > screen_width())
        target_object.x = 0;
    if(target_object.x < sprite_width(s) * -1)
        target_object.x = screen_width();
    if(target_object.y > screen_height())
        target_object.y = 0;
    if(target_object.y < sprite_height(s) * -1)
        target_object.y = screen_height();
}

// handle key strokes from the user
void handle_input(game_data &game)
{
    // Handle movement - rotating left/right and changes in velocity
    float rotation = sprite_rotation(game.player.main_sprite);

    // Allow rotation with A/D keys
    if (key_down(A_KEY))
        sprite_set_rotation(game.player.main_sprite, rotation - PLAYER_ROTATE_SPEED);

    if (key_down(D_KEY))
        sprite_set_rotation(game.player.main_sprite, rotation + PLAYER_ROTATE_SPEED);

    // Modify velocity
    sprite_hide_layer(game.player.main_sprite, 1); // sprite layer representing thrust
    if (key_down(W_KEY)){
        // add to velocity depending on direction of thrust and existing velocity
        game.player.velocity.x += cos(rotation*M_PI/180) * PLAYER_SPEED;
        game.player.velocity.y += sin(rotation*M_PI/180) * PLAYER_SPEED;
        sprite_show_layer(game.player.main_sprite, 1); // show thrust layer
        play_sound_effect(sound_effect_named("thrust")); // play thrust sound effect
    }

    // Hyperspace
    if (key_typed(S_KEY)){
        free_sprite(game.player.main_sprite); // remove players sprite
        spawn_player(game.player,false); // spawn new sprite randomly on screen
    }

    // Shooting
    if (key_down(SPACE_KEY)){
        if(timer_ticks(game.player.shoot_cooldown) > COOLDOWN_TIME){ // shooting cooldown, prevents spam...
            reset_timer(game.player.shoot_cooldown); // reset cooldown timer
            start_timer(game.player.shoot_cooldown); // start cooldown timer
            game.bullets.push_back(new_bullet(game.player)); // create new bullet
            play_sound_effect(sound_effect_named("shoot")); // play sound effect
        }
    }   
}

// check if there's a collision between asteroid and player or bullet
void check_collisions(game_data &game)
{
    for(int i = 0; i < game.asteroids.size(); i++){ // traverse through asteroids
        // check for collision between player and current asteroid
        if(sprite_collision(game.player.main_sprite, game.asteroids[i].main_sprite)){
            if(timer_ticks(game.player.player_timer) > IMMUNITY_TIME){ // check if player's immune?
                play_sound_effect(sound_effect_named("explode")); // play sound effect
                free_sprite(game.player.main_sprite); // delete players sprite
                game.player.lives--; // remove a life               

                if(game.player.lives > 0){ // player still has a life
                    reset_timer(game.player.player_timer); // reset timer for spawn delay
                    start_timer(game.player.player_timer);
                    bool player_spawned = false;
                    while(!player_spawned){ // stop game for player visual feedback
                        if(timer_ticks(game.player.player_timer) > SPAWN_DELAY){
                            spawn_player(game.player,true); // spawn new player centred in screen
                            player_spawned = true;
                        }
                    }
                }
                else{ // no lives, end game, show menu
                    stop_music(); // kill annoying music :P
                    game.menu_active = true; // show menu
                }
            }
            else{
                reset_timer(game.player.player_timer); // reset timer for immunity
                start_timer(game.player.player_timer); // keeps player immune for a while longer
            }
        }
        else{ // check for collision between bullets and asteroids
            for(int j = 0; j < game.bullets.size(); j++){ // traverse active bullets
                // check current bullet and current asteroid for collision
                if(sprite_collision(game.bullets[j].main_sprite, game.asteroids[i].main_sprite)){
                    play_sound_effect(sound_effect_named("explode")); // play sound effect
                    handle_bullet_collision(game, j, i); // process the collision
                }
            }
        }
    }
}

// handles a collision between a bullet and asteroid
void handle_bullet_collision(game_data &game, int bullet_idx, int roid_idx){
    free_sprite(game.bullets[bullet_idx].main_sprite); // delete bullet sprite
    timer tmp_timer = timer_named(game.bullets[bullet_idx].timer_name); // reference to bullets timer
    free_timer(tmp_timer); // delete associated timer
    game.bullets.erase(game.bullets.begin() + bullet_idx); // remove bullet from vector

    // delete asteroid
    int roid_level = game.asteroids[roid_idx].roid_level; // current level|size of the asteroid
    float scale = 0;    

    // sort out scoring (and scaling if applicable)
    switch (roid_level)
    {
        case 3: // large asteroid hit
            game.player.score += 20;
            game.player.free_life_score += 20;
            scale = .7;
            break;
        case 2: // medium asteroid hit
            game.player.score += 50;
            game.player.free_life_score += 50;
            scale = .4;
            break;
        case 1: // small asteroid hit
            game.player.score += 100;
            game.player.free_life_score += 100;
            break;
    }

    // handle free life
    if(game.player.free_life_score > FREE_LIFE){
       game.player.free_life_score -=  FREE_LIFE; // negate the free life value
       game.player.lives++; // add a life
    }

    // spawn more asteroids if required
    if(scale > 0){
        roid_level--; // decrement the level|size of the asteroid
        for(int i = 0; i < 2; i++){ // spawn another two asteroids
            game.asteroids.push_back(
                new_roid(
                    sprite_x(game.asteroids[roid_idx].main_sprite), 
                    sprite_y(game.asteroids[roid_idx].main_sprite), 
                    roid_level, 
                    scale,
                    game.player
                )
            );
        }
    }

    free_sprite(game.asteroids[roid_idx].main_sprite); // delete asteroid sprite
    game.asteroids.erase(game.asteroids.begin() + roid_idx); // delete asteroid from vector

    // check if there are no more active asteroids, if not, create new level
    if(game.asteroids.size() == 0){
        game.asteroid_count++; // increment amount of asteroids per level
        new_level(game);
    }
}

// draw the hud
void draw_hud(const player_data &player){
    draw_text(to_string(player.score), COLOR_RED, font_named("atari"), 55, 55, 20, option_to_screen());
    draw_text(to_string(player.lives), COLOR_RED, font_named("atari"), 55, screen_width()-100, 20, option_to_screen());
}

// load the games menu|instructions
void load_menu(game_data &game){
    clear_screen(COLOR_BLACK); // clear screen
    draw_bitmap(bitmap_named("back"), 0, 0, option_to_screen()); // draw background image
    
    // update roids (for asthetics)
    for(int i = 0; i < game.asteroids.size(); i++)
    { 
        update_roid(game.asteroids[i]);
    }
    // draw asteroids 
    for(int i = 0; i < game.asteroids.size(); i++)
    { 
        draw_roid(game.asteroids[i]);
    }

    draw_hud(game.player);
    bitmap menu = bitmap_named("menu");
    draw_bitmap(menu, screen_width() / 2 - (bitmap_width(menu)/2), 
        screen_height() / 2 - (bitmap_height(menu)/2), option_to_screen());
    refresh_screen(60);
}

// handle key strokes whilst in the menu
bool menu_input(game_data &game){
    // Escape key starts a new game
    if (key_down(ESCAPE_KEY)){
        //write_line("START GAME");
        play_music(load_music("background", "background.mp3"),-1); // play annoying background music :P
        game = new_game(); // create new game
        return false;
    }
    else
         return true;
    
}


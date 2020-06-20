#include "player.h"
#include "game.h"
//#include "bullets.h"
#include "splashkit.h"



// create new player
player_data new_player()
{
    player_data result;
    
    // create and start cooldown timer
    result.shoot_cooldown = create_timer("shoot_cooldown");
    reset_timer(result.shoot_cooldown);
    start_timer(result.shoot_cooldown);

    // create and start player timer
    result.player_timer = create_timer("player_timer");
    reset_timer(result.player_timer);
    start_timer(result.player_timer);

    spawn_player(result, true); // create player sprite and place it on the screen

    result.lives = PLAYER_LIVES;
    result.score = 0;
    result.free_life_score = 0;
    
    return result;
}

// spawn player
void spawn_player(player_data &player, bool centred)
{
    player.main_sprite = create_sprite(bitmap_named("ship"));   // default ship image
    sprite_add_layer(player.main_sprite, bitmap_named("shipt"), "shipt"); // ship with thrust
    sprite_add_layer(player.main_sprite, bitmap_named("shipi"), "shipi"); // ship halo reflecting immunity
    sprite_set_rotation(player.main_sprite, -90); // point ship north
    player.velocity.x = 0; // set initial velocity to zero
    player.velocity.y = 0; // set initial velocity to zero
    if(centred){ // new players are spawned in the centre of the screen
        player.current_position.x = int((screen_width() - sprite_width(player.main_sprite)) / 2);
        player.current_position.y = int((screen_height() - sprite_height(player.main_sprite)) / 2);
        reset_timer(player.player_timer); // reset timer for immunity
        start_timer(player.player_timer);
        sprite_show_layer(player.main_sprite, 2); // show halo immunity layer
    }
    else{ // hyperspace used, place the ship randomly without immunity
        player.current_position.x = rnd(screen_width());
        player.current_position.y = rnd(screen_height());
    }
    sprite_set_x(player.main_sprite, player.current_position.x); // set sprite to saved location
    sprite_set_y(player.main_sprite, player.current_position.y);
}

// draw player
void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.main_sprite);
}

// update player
void update_player(player_data &player_to_update)
{
    // Apply movement based on rotation and velocity
    player_to_update.current_position.x += player_to_update.velocity.x;
    player_to_update.current_position.y += player_to_update.velocity.y;

    // screen wrapping, check if sprite is off screen. if so, place it on other side.
    screen_wrap(player_to_update.current_position, player_to_update.main_sprite);

    // update sprites location to reflect change
    sprite_set_x(player_to_update.main_sprite, player_to_update.current_position.x);
    sprite_set_y(player_to_update.main_sprite, player_to_update.current_position.y);
}
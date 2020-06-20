#include "splashkit.h"
#include "game.h"
#include "bullets.h"


using namespace std;

// used to get M_PI and math functions cosine and sine
#define _USE_MATH_DEFINES 
#include <math.h>


bullet_data new_bullet(const player_data &player)
{
    bullet_data result;
    float rotation = sprite_rotation(player.main_sprite); // save players rotation
    result.main_sprite = create_sprite(bitmap_named("bullet"));
    result.current_position = player.current_position;

    // updating velocity based on existing player velocity and direction
    result.velocity.x = cos(rotation*M_PI/180) * BULLET_MODIFIER + player.velocity.x;
    result.velocity.y = sin(rotation*M_PI/180) * BULLET_MODIFIER + player.velocity.y;
    
    // set bullet to players location
    sprite_set_x(result.main_sprite, result.current_position.x);
    sprite_set_y(result.main_sprite, result.current_position.y);

    create_timer(result); // creating the cooldown timer

    return result;
}

// update bullet
void update_bullet(bullet_data &bullet_to_update)
{
    
        // Apply movement based on rotation and velocity
        bullet_to_update.current_position.x += bullet_to_update.velocity.x;
        bullet_to_update.current_position.y += bullet_to_update.velocity.y;

        // screen wrapping, if sprite is off screen place it on other side.
        screen_wrap(bullet_to_update.current_position, bullet_to_update.main_sprite);

        // set sprites location to the updated coordinates
        sprite_set_x(bullet_to_update.main_sprite, bullet_to_update.current_position.x);
        sprite_set_y(bullet_to_update.main_sprite, bullet_to_update.current_position.y);
    
}

// draw bullet
void draw_bullet(const bullet_data &bullet_to_draw)
{    
        draw_sprite(bullet_to_draw.main_sprite);   
}

// create the timer so bullet is removed after a certain amount of time
void create_timer(bullet_data &this_bullet)
{
    // creating a unique timer name
    int number = 0;
    string temp_name = "timer";
    string timer_name;
    
    bool not_unique_name = true;
    while(not_unique_name){
        number++;
        timer_name = temp_name + to_string(number);
        if(!has_timer(timer_name))
            not_unique_name = false;
    }
    
    // create the timer using the unique timer name
    this_bullet.timer_name = timer_name;
    this_bullet.bullet_timer = create_timer(timer_name);
    reset_timer(this_bullet.bullet_timer);
    start_timer(this_bullet.bullet_timer);
}
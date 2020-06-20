#include "splashkit.h"
#include "asteroids.h"
#include "game.h"
#include "player.h"

using namespace std;



/**
 * The asteroid bitmap function converts a asteroid style into a 
 * bitmap that can be used.
 * 
 * Not exposed by the header.
 * 
 * @param style     The style of asteroid
 * @return          The bitmap matching this style
 **/
bitmap roid_bitmap(roid_style style)
{
    switch (style)
    {
        case BLUE_SPIKEY:
            return bitmap_named("blue_spikey");
        case GREEN_SMOOTH:
            return bitmap_named("green_smooth");
        case GREEN_SPIKEY:
            return bitmap_named("green_spikey");
        case ORANGE_SMOOTH:
            return bitmap_named("orange_smooth");
        case PINK_SMOOTH:
            return bitmap_named("pink_smooth");
        case PURPLE_SMOOTH:
            return bitmap_named("purple_smooth");
        case RED_SMOOTH:
            return bitmap_named("red_smooth");
        case YELLOW_SPIKEY:
            return bitmap_named("yellow_spikey");
    }
}


roid_data new_roid(float x, float y, int roid_level, float scale, player_data &player)
{    
    roid_data result;
    //bitmap default_bitmap = roid_bitmap(static_cast<roid_style>(rnd(8)));
    result.main_sprite = create_sprite(roid_bitmap(static_cast<roid_style>(rnd(8))));
    sprite_set_scale(result.main_sprite, scale);
    
    result.roid_level = roid_level;

    if(scale == 1){ // fresh asteroid placement, reset player immunity
        reset_timer(player.player_timer);
        start_timer(player.player_timer);
        sprite_show_layer(player.main_sprite, 2);
    }

    // Set a random initial asteroid direction
    sprite_set_rotation(result.main_sprite, rnd(360));
    
    // Set asteroids initial x & y speed (.1 to 1)
    sprite_set_dx(result.main_sprite, .5);

    // Position in asteroid according to supplied parameters

    sprite_set_x(result.main_sprite, x);
    sprite_set_y(result.main_sprite, y);

    return result;
}

// draw the asteroid to the screen
void draw_roid(const roid_data &roid_to_draw)
{
    draw_sprite(roid_to_draw.main_sprite);
}

//  update the asteroid
void update_roid(roid_data &roid_to_update)
{        
    // screen wrapping
    point_2d tmp_position = sprite_position(roid_to_update.main_sprite); // get existing position
    screen_wrap(tmp_position, roid_to_update.main_sprite); //if sprite is off the screen place it on other side
    sprite_set_position(roid_to_update.main_sprite, tmp_position);
    
    // Apply movement based on rotation and velocity in the sprite
    update_sprite(roid_to_update.main_sprite);
}


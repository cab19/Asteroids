#ifndef ASTEROIDS_ROIDS
#define ASTEROIDS_ROIDS

#include "splashkit.h"
#include "player.h"

//#include <vector>

using namespace std;


/**
 * Selection of different styled asteroids
 * Adjusts the image used.
 * 
 **/
enum roid_style
{
    BLUE_SPIKEY,
    GREEN_SMOOTH,
    GREEN_SPIKEY,
    ORANGE_SMOOTH,
    PINK_SMOOTH,
    PURPLE_SMOOTH,
    RED_SMOOTH,
    YELLOW_SPIKEY
};

/**
 * The asteroid data keeps track of all of the information related to the asteroid.
 * 
 * @field   main_sprite     The asteroids's sprite - used to track position and movement
 * @field   score           The current score for the player
 * 
 **/
struct roid_data
{
    sprite     main_sprite;
    int        roid_level;
};

/**
 * The new roid function creates and returns a new asteroid
 * 
 * @field   x           the x position of the created asteroid
 * @field   y           the y position of the created asteroid
 * @field  level       the level of the asteroid, 3 is large, 2 medium, 1 small
 * @field   scale       the scale of the asteroid bitmap, to represent the three sizes
 * @field   player      reference to player, used to reset the immunity timer when fresh asteroids are created
 *
 **/
roid_data new_roid(float x, float y, int level, float scale, player_data &player); // reset the level of the asteroid to 3

/**
 * The draw roid function draws the sprite to the screen
 * 
 * @field   roid_to_draw    the asteroid to draw
 * 
 **/
void draw_roid(const roid_data &roid_to_draw);

/**
 * The update roid function changes the roids position based on the sprites velocity and direction
 * 
 * @field   roid_to_update    the asteroid to update
 * 
 **/
void update_roid(roid_data &roid_to_update);

#endif
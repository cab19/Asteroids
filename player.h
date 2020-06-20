#ifndef ASTEROIDS_PLAYER
#define ASTEROIDS_PLAYER

#include "splashkit.h"

using namespace std;

#define PLAYER_SPEED .15
#define PLAYER_ROTATE_SPEED 5
#define PLAYER_LIVES 4
#define FREE_LIFE 5000
#define COOLDOWN_TIME 400
#define IMMUNITY_TIME 3000
#define SPAWN_DELAY 2500

/**
 * The player data keeps track of all of the information related to the player.
 * 
 * @field   main_sprite             The player's sprite - used to track position and movement
 * @field   score                   The current score for the player
 * @field   free_life_score         Used to keep track of progress toward free life
 * @field   lives                   Lives left
 * @field   current_position        Point 2d representing the players current position
 * @field   velocity                Point 2d representing the players current velocity
 * @field   shoot_cooldown          Timer to prevent player spamming bullets
 * @field   player_timer            Dual use, used for both immunity and spawn delay
 */
struct player_data
{
    sprite      main_sprite;
    int         score;
    int         free_life_score;
    int         lives;
    point_2d    current_position;
    point_2d    velocity;
    timer       shoot_cooldown;
    timer       player_timer;   
};

/**
 * Creates a new player and sets up initial values and timers.
 * 
 * @returns     The new player data
 **/
player_data new_player();

/**
 * Creates the sprite, adds bitmap layers, places sprite either in the centre (new ship) or randomly (hyperspace)
 *
 * @field   player            Reference to the player
 * @field   centred           Denotes whether the sprite should be placed centrally.
 * 
 **/
void spawn_player(player_data &player, bool centred);


/**
 * Draws the player to the screen. 
 * 
 * @field player_to_draw    The player to draw to the screen
 **/
void draw_player(const player_data &player_to_draw);

/**
 * Updates the player using the existing position and velocity
 * 
 * @field player_to_update      The player being updated
 **/
void update_player(player_data &player_to_update);

#endif 
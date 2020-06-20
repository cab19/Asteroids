#ifndef ASTEROIDS_BULLETS
#define ASTEROIDS_BULLETS

//#include "splashkit.h"
//#include "game.h"
#include "player.h"

//#include <vector>

using namespace std;

#define BULLET_MODIFIER 30
#define BULLET_LIFE 3000

/**
 * The bullet data keeps track of all of the information related to each bullet.
 * 
 * @field   main_sprite                 The bullets sprite
 * @field   current_position            Point 2d representing the bullets current position
 * @field   velocity                    Point 2d representing the bullets current velocity
 * @field   timer_name                  Holds a unique name, so the timer can be freed when bullet is
 * @field   bullet_timer                The actual timer, bullets are removed when their life expires
 *
 **/
struct bullet_data
{
    sprite      main_sprite;
    point_2d    current_position;
    point_2d    velocity;
    string      timer_name;
    timer       bullet_timer;
};

/**
 * The new bullet function creates and returns a new bullet
 * 
 * @field   player      Reference to player so bullet can fire forward of the player
 *
 **/
bullet_data new_bullet(const player_data &player);

/**
 * The update bullet function changes the bullets position using the current position and velocity data
 * 
 * @field   bullet_to_update    the asteroid to update
 * 
 **/
void update_bullet(bullet_data &bullet_to_update);

/**
 * The draw bullet function draws the sprite to the screen
 * 
 * @field   bullet_to_draw    the bullet to draw
 * 
 **/
void draw_bullet(const bullet_data &bullet_to_draw);

/**
 * The create timer function creates a timer using a uniquely generated name
 * 
 * @field   this_bullet    the bullet associated with the timer being created
 * 
 **/
void create_timer(bullet_data &this_bullet);

#endif
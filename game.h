#ifndef ASTEROIDS_GAME
#define ASTEROIDS_GAME

#include "splashkit.h"
#include "asteroids.h"
#include "bullets.h"
#include "player.h"

#include <vector>

using namespace std;


/**
 * The game data keeps track of all of the information related to the game.
 * 
 * @field   player              Reference to the player
 * @field   bullets             A vector tracking all active bullets
 * @field   asteroids           A vector tracking all active asteroids
 * @field   asteroid_count      The current amount of asteroids
 * @field   menu_active         Whether the menu is active or not
 * 
 **/

struct game_data
{
    player_data             player;
    vector<bullet_data>     bullets;
    vector<roid_data>       asteroids;
    int                     asteroid_count;
    bool                    menu_active;
};

/**
 * Setup the level, reset the timers and add planets to planets vector.
 * 
 * @field   game            reference to the game
 * 
 **/
void new_level(game_data &game);


/**
 * 
 * Creates game_data variable, creates a player, set asteroids creates a new level. 
 * Returns the game_data
 * 
 **/
game_data new_game();

/**
 * 
 * Uses the game variable to draw all elements (bullets, asteroids, player, hud)
 * 
 * @field   &game    reference to the game_data variable
 * 
 **/
void draw_game(const game_data &game);

/**
 * 
 * Updates all game elements and checks for collisions
 * 
 **/
void update_game(game_data &game);

/**
 * 
 * Checks whether the supplied object is beyond the screen bounds
 * places objects outside the screen onto the opposite side.
 * 
 * @field target_object     reference to object to be checked
 * @field s                 reference to the objects sprite
 * 
 **/
void screen_wrap(point_2d &target_object, sprite s);

/**
 * 
 * Handles input from the player within the game, ie thrust, rotation of player, shooting, etc.
 * 
 * @field game               reference to the current game
 * 
 **/
void handle_input(game_data &game);


/**
 * 
 * Check if there's a collision between asteroid and a player or bullet
 * 
 * @field game               reference to the current game
 * 
 **/
void check_collisions(game_data &game);

/**
 * 
 * Handles a collision between bullet and an asteroid
 * 
 * @field game               reference to the current game
 * @field bullet_idx         reference to index of the bullet in the vector
 * @field roid_idx           reference to index of the asteroid in the vector
 * 
 **/
void handle_bullet_collision(game_data &game, int bullet_idx, int roid_idx);

/**
 * 
 * Draw the players HUD, which consists of their score and lives left
 * 
 * @field player             reference to the player
 * 
 **/
void draw_hud(const player_data &player);

/**
 * 
 * Created the menu, containing the instructions
 * 
 * @field game             reference to the current game
 * 
 **/
void load_menu(game_data &game);

/**
 * 
 * Handle key strokes whilst the menu is active, only cares about the escape key which starts the game
 * 
 * @field game             reference to the current game
 * 
 **/
bool menu_input(game_data &game);

#endif
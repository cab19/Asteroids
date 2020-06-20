#include "splashkit.h"
#include "game.h"

using namespace std;

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1000

/**
 * Load the game images, sounds, etc.
 **/
void load_resources()
{
    load_resource_bundle("game_bundle", "asteroids.txt");
}

// main...
int main()
{
    open_window("ASTEROIDS", SCREEN_WIDTH, SCREEN_HEIGHT); // create window
    load_resources(); // load resource bundle

    game_data game = new_game(); // create a new game
    
    while (not quit_requested())
    {
        process_events();
        
        if(game.menu_active){ // if menu is active
            load_menu(game); // load the menu
            game.menu_active = menu_input(game); // if escape is pressed, returns false and starts the game
        }
        else{ // game is active
            draw_game(game);
            handle_input(game);
            update_game(game);
        }

              
    }

    return 0;
}
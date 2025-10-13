#include "game.h"

#include "ui.h"

void game_init(void)
{

}


WindowState game_run(SDL_Surface *surface, Ball *ball, Ball *trajectory, Racket *racket, int trajectory_entry_count, uint32_t last_time)
{
    uint32_t current_time = SDL_GetTicks();
    double delta_time = (current_time - last_time) / 1000.0;  // Time (in seconds)
    last_time = current_time;

    //Racket movement based on time
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J]) {
        racket->racket_pos += racket->racket_speed * delta_time;  //Moves down
    }
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
        racket->racket_pos -= racket->racket_speed * delta_time;  //Moves up
    }

    //Ensures the racket stays within the screen boundaries
    if(racket->racket_pos < 0) racket->racket_pos = 0;
    if(racket->racket_pos + 100 > SCREEN_HEIGHT) racket->racket_pos = SCREEN_HEIGHT - 100;

    ball_trajectory_fill(surface, trajectory, trajectory_entry_count);
    ball_fill(surface, &ball, WHITE);
    table_create(surface);
    racket_create(surface, racket);

    ball_physics(ball, racket);
    ball_trajectory_update(trajectory, &ball, trajectory_entry_count);

    if(trajectory_entry_count < TRAJECTORY_LENGTH) ++trajectory_entry_count;

    return WINDOW_GAME;
}

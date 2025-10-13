#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "ui.h"


//Main function
int main(void)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    if(surface == NULL) {
        printf("ERROR!!!");
    }

    //Generates the initial v_y for the ball
    double random_num = (rand() % 3) - 1;
    double random_vel = 1;
    if(random_num == 1 || random_num == -1) random_vel = random_num;

    Ball ball = {(SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 20, 5, random_vel};
    Ball trajectory[TRAJECTORY_LENGTH];
    Racket racket = (Racket) {(SCREEN_HEIGHT / 2) - 50, 1000.0};
    int trajectory_entry_count = 0;

    SDL_Rect erase_rect = (SDL_Rect) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Event event;
    int simulation_running = 1;

    //Variable to atualize the position of the racket
    uint32_t last_time = SDL_GetTicks();  //Controls time variation

    WindowState current_window = WINDOW_GAME;

    while(simulation_running != 0) {
        // Polling of events
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_SPACE) {
                    simulation_running = 0;
                }
            }
        }

        SDL_FillRect(surface, &erase_rect, WINDOW_COLOR);
        {
            switch(current_window) {
                case WINDOW_MENU:
                    break;
                case WINDOW_GAME:
                    current_window = game_run(surface, &ball, trajectory, &racket, &trajectory_entry_count, &last_time);
                    break;
                case WINDOW_PAUSE:
                    break;
                default:
                    break;
            }

//            uint32_t current_time = SDL_GetTicks();
//            double delta_time = (current_time - last_time) / 1000.0;  // Time (in seconds)
//            last_time = current_time;
//
//            //Racket movement based on time
//            if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J]) {
//                racket.racket_pos += racket.racket_speed * delta_time;  //Moves down
//            }
//            if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
//                racket.racket_pos -= racket.racket_speed * delta_time;  //Moves up
//            }
//
//            //Ensures the racket stays within the screen boundaries
//            if(racket.racket_pos < 0) racket.racket_pos = 0;
//            if(racket.racket_pos + 100 > SCREEN_HEIGHT) racket.racket_pos = SCREEN_HEIGHT - 100;
//
//            ball_trajectory_fill(surface, trajectory, trajectory_entry_count);
//            ball_fill(surface, ball, WHITE);
//            table_create(surface);
//            racket_create(surface, racket);
//
//            ball_physics(&ball, &racket);
//            ball_trajectory_update(trajectory, ball, trajectory_entry_count);
//
//            if(trajectory_entry_count < TRAJECTORY_LENGTH) ++trajectory_entry_count;

            //Updates the window
            SDL_UpdateWindowSurface(window);
            SDL_Delay(1);  //Adjusts the delay if needed to control the FPS
        }
    }

    SDL_Quit();
    return 0;
}


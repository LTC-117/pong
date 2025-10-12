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


//Table of values
#define SCREEN_WIDTH       900.0
#define SCREEN_HEIGHT      600.0
#define TRAJECTORY_LENGTH      20
#define WHITE       0xffffffff
#define WINDOW_COLOR   0x00000011


/*Type definitions:*/

//Struct for circle generation
typedef struct Ball{
    double x;
    double y;
    double radius;
    double v_x;
    double v_y;
} Ball;


//Struct for racket generation
typedef struct Racket{
    double racket_pos;
    double racket_speed;
} Racket;


//Trajectory color colection
Uint32 g_trajectory_colors[] = {
    0xFF0000,0xFF1400,0xFF1E00,0xFF2300,0xFF2800,0xFF2D00,
    0xFF3200,0xFF4600,0xFF5000,0xFF5500,0xFF5A00,0xFF5F00,
    0xFF9600,0xFFAA00,0xFFB400,0xFFB900,0xFFBE00,0xFFC300,
    0xFFCD00,0xFFE600,0xFFF000,0xFFFF00,0xFFFF00
};


//Function for generating the table details
void fill_table(SDL_Surface *surface) {
    //Faixa central:
    SDL_Rect central_limit = (SDL_Rect) {(SCREEN_WIDTH/2), 0, 1, SCREEN_HEIGHT};
    SDL_FillRect(surface, &central_limit, WHITE);

    //Faixa lateral direita:
    SDL_Rect right_limit = (SDL_Rect) {(SCREEN_WIDTH-1), 0, 1, SCREEN_HEIGHT};
    SDL_FillRect(surface, &right_limit, WHITE);

    //Faixa superior:
    SDL_Rect upper_limit = (SDL_Rect) {0, 0, SCREEN_WIDTH, 1};
    SDL_FillRect(surface, &upper_limit, WHITE);

    //Faixa inferior :
    SDL_Rect lower_limit = (SDL_Rect) {0, (SCREEN_HEIGHT-1), SCREEN_WIDTH, 1};
    SDL_FillRect(surface, &lower_limit, WHITE);
}


//Generates the ball
void fill_ball(SDL_Surface *surface, Ball circle, int color) {
    double low_x = circle.x - circle.radius;
    double low_y = circle.y - circle.radius;
    double high_x = circle.x + circle.radius;
    double high_y = circle.y + circle.radius;

    double radius_squared = circle.radius * circle.radius;

    for(double x=low_x; x < high_x; x++) {
        for(double y=low_y; y < high_y; y++) {
            //Is coordinate within circle?
            double center_distance = (x-circle.x)*(x-circle.x) + (y-circle.y)*(y-circle.y);
            if(center_distance < radius_squared) {
                SDL_Rect pixel = (SDL_Rect) {x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}


//Generates the playing bar
void fill_racket(SDL_Surface *surface, Racket racket) {
    // Preencher o racket na nova posição
    SDL_Rect real_racket = (SDL_Rect) {0, racket.racket_pos, 30, 100};
    SDL_FillRect(surface, &real_racket, WHITE);
}


//Generates the trajectory for the ball
void fill_trajectory(SDL_Surface *surface, Ball trajectory[TRAJECTORY_LENGTH], int current_trajectory_index) {
    for(int i=0; i<current_trajectory_index; i++) {
        trajectory[i].radius = i;
        fill_ball(surface, trajectory[i], g_trajectory_colors[i]);
    }
}


//Updates each part of the trajectory accorging to the position of the ball
void update_trajectory(Ball trajectory[TRAJECTORY_LENGTH], struct Ball circle, int current_index) {
    if(current_index >= TRAJECTORY_LENGTH) {
        //shift array - write the circle at the end of the array
        Ball trajectory_copy[TRAJECTORY_LENGTH];
        for(int i=0; i<TRAJECTORY_LENGTH; i++) {
            if(i > 0 && i < TRAJECTORY_LENGTH)
                trajectory_copy[i] = trajectory[i+1];
        }
        for(int i=0; i<TRAJECTORY_LENGTH; i++) {
            trajectory[i] = trajectory_copy[i];
        }
        trajectory[current_index] = circle;
    }
    else{
        trajectory[current_index] = circle;
    }
}


//Defines how the ball will interact with the environment
void game_physics(Ball *circle, Racket *racket) {
    //How do we calculate the new position?
    circle->x += circle->v_x;
    circle->y += circle->v_y;

    //Y axis:
    if( (circle->y + circle->radius) > SCREEN_HEIGHT ) {
        circle->y = SCREEN_HEIGHT - circle->radius;
        if(circle->v_y>0) circle->v_y+=0.1; else circle->v_y-=0.1;
        circle->v_y = -(circle->v_y);
    }
    if( (circle->y - circle->radius) < 0) {
        circle->y = circle->radius;
        if(circle->v_y>0) circle->v_y+=0.1; else circle->v_y-=0.1;
        circle->v_y = -(circle->v_y);
    }
    //X axis:
    if( (circle->x + circle->radius) > SCREEN_WIDTH ) {
        circle->x = SCREEN_WIDTH - circle->radius;
        if(circle->v_x>0) circle->v_x+=0.1; else circle->v_x-=0.1;
        circle->v_x = -(circle->v_x);
    }
    if( ((circle->x - circle->radius) < 30) && ((circle->x - circle->radius) > 0)) {
        if(circle->y > racket->racket_pos) {
            if(circle->y < racket->racket_pos + 100) {
                circle->x = circle->radius + 30;
                if(circle->v_x>0) circle->v_x+=0.1; else circle->v_x-=0.1;
                circle->v_x = -(circle->v_x);
            }
        }
    }
}


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
    Uint32 last_time = SDL_GetTicks();  //Controls time variation

    while(simulation_running != 0) {
        Uint32 current_time = SDL_GetTicks();
        double delta_time = (current_time - last_time) / 1000.0;  // Time (in seconds)
        last_time = current_time;

        // Polling of events
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_SPACE) {
                    simulation_running = 0;
                }
            }
        }

        //Racket movement based on time
        if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J]) {
            racket.racket_pos += racket.racket_speed * delta_time;  //Moves down
        }
        if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K]) {
            racket.racket_pos -= racket.racket_speed * delta_time;  //Moves up
        }

        //Ensures the racket stays within the screen boundaries
        if(racket.racket_pos < 0) racket.racket_pos = 0;
        if(racket.racket_pos + 100 > SCREEN_HEIGHT) racket.racket_pos = SCREEN_HEIGHT - 100;

        //Filling the screen and the functions
        SDL_FillRect(surface, &erase_rect, WINDOW_COLOR);
        fill_trajectory(surface, trajectory, trajectory_entry_count);
        fill_ball(surface, ball, WHITE);
        fill_table(surface);
        fill_racket(surface, racket);

        //Updates the window
        SDL_UpdateWindowSurface(window);
        SDL_Delay(1);  //Adjusts the delay if needed to control the FPS

        game_physics(&ball, &racket);
        update_trajectory(trajectory, ball, trajectory_entry_count);

        if(trajectory_entry_count < TRAJECTORY_LENGTH) ++trajectory_entry_count;
    }

    SDL_Quit();
    return 0;
}


#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <stddef.h>
#include <stdint.h>

#define TRAJECTORY_LENGTH      20

//Struct for ball generation
typedef struct Ball {
    double x;
    double y;
    double radius;
    double v_x;
    double v_y;
} Ball;


//Struct for racket generation
typedef struct Racket {
    double racket_pos;
    double racket_speed;
} Racket;

//Trajectory color colection
uint32_t g_trajectory_colors[] = {
    0xFF0000,0xFF1400,0xFF1E00,0xFF2300,0xFF2800,0xFF2D00,
    0xFF3200,0xFF4600,0xFF5000,0xFF5500,0xFF5A00,0xFF5F00,
    0xFF9600,0xFFAA00,0xFFB400,0xFFB900,0xFFBE00,0xFFC300,
    0xFFCD00,0xFFE600,0xFFF000,0xFFFF00,0xFFFF00
};

// Ball
void ball_fill(SDL_Surface *surface, Ball circle, int color);
void ball_trajectory_fill(SDL_Surface *surface, Ball trajectory[TRAJECTORY_LENGTH], int current_trajectory_index);
void ball_trajectory_update(Ball trajectory[TRAJECTORY_LENGTH], struct Ball circle, int current_index);
void ball_physics(Ball *circle, Racket *racket);

// Table
void fill_table(SDL_Surface *surface);

// Racket
void fill_racket(SDL_Surface *surface, Racket racket);

#endif

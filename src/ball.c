#include "game.h"
#include "ui.h"

// TODO: Implement new features and visuals for the ball


//Generates the ball
void ball_fill(SDL_Surface *surface, Ball circle, int color)
{
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


//Generates the trajectory for the ball
void ball_trajectory_fill(SDL_Surface *surface, Ball trajectory[TRAJECTORY_LENGTH], int current_trajectory_index)
{
    for(int i=0; i<current_trajectory_index; i++) {
        trajectory[i].radius = i;
        ball_fill(surface, trajectory[i], g_trajectory_colors[i]);
    }
}


//Updates each part of the trajectory according to the position of the ball
void ball_trajectory_update(Ball trajectory[TRAJECTORY_LENGTH], struct Ball circle, int current_index)
{
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
void ball_physics(Ball *circle, Racket *racket)
{
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

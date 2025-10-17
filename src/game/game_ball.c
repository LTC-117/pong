#include "game.h"
#include "ui.h"


// TODO: Implement new features and visuals for the ball


//Generates the ball
void ball_fill(SDL_Surface *surface, Ball ball, int color)
{
    double low_x = ball.x - ball.radius;
    double low_y = ball.y - ball.radius;
    double high_x = ball.x + ball.radius;
    double high_y = ball.y + ball.radius;

    double radius_squared = ball.radius * ball.radius;

    for(double x = low_x; x < high_x; x++) {
        for(double y = low_y; y < high_y; y++) {
            //Is coordinate within ball?
            double center_distance = (x - ball.x)*(x - ball.x) + (y - ball.y)*(y - ball.y);
            if(center_distance < radius_squared) {
                SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}


//Generates the trajectory for the ball
void ball_trajectory_fill(SDL_Surface *surface, Ball trajectory[TRAJECTORY_LENGTH], int current_trajectory_index)
{
    for(int i = 0; i < current_trajectory_index; i++) {
        trajectory[i].radius = i;
        ball_fill(surface, trajectory[i], g_trajectory_colors[i]);
    }
}


//Updates each part of the trajectory according to the position of the ball
void ball_trajectory_update(Ball trajectory[TRAJECTORY_LENGTH], struct Ball ball, int current_index)
{
    if(current_index >= TRAJECTORY_LENGTH) {
        //shift array - write the ball at the end of the array
        Ball trajectory_copy[TRAJECTORY_LENGTH];
        for(int i = 0; i < TRAJECTORY_LENGTH; i++) {
            if(i > 0 && i <= TRAJECTORY_LENGTH)
                trajectory_copy[i] = trajectory[i + 1];
        }
        for(int i = 0; i < TRAJECTORY_LENGTH; i++) {
            trajectory[i] = trajectory_copy[i];
        }
        trajectory[current_index] = ball;
    }
    else{
        trajectory[current_index] = ball;
    }
}


//Defines how the ball will interact with the environment
void ball_physics(Ball *ball, Racket *racket)
{
    //How do we calculate the new position?
    ball->x += ball->v_x;
    ball->y += ball->v_y;

    //Y axis:
    if( (ball->y + ball->radius) > SCREEN_HEIGHT ) {
        ball->y = SCREEN_HEIGHT - ball->radius;
        if(ball->v_y > 0) ball->v_y += 0.1;
        else ball->v_y -= 0.1;
        ball->v_y = -(ball->v_y);
    }
    if( (ball->y - ball->radius) < 0) {
        ball->y = ball->radius;
        if(ball->v_y > 0) ball->v_y += 0.1;
        else ball->v_y -= 0.1;
        ball->v_y = -(ball->v_y);
    }
    //X axis:
    if( (ball->x + ball->radius) > SCREEN_WIDTH ) {
        ball->x = SCREEN_WIDTH - ball->radius;
        if(ball->v_x > 0) ball->v_x += 0.1;
        else ball->v_x -= 0.1;
        ball->v_x = -(ball->v_x);
    }
    if( ((ball->x - ball->radius) < 30) && ((ball->x - ball->radius) > 0)) {
        if(ball->y > racket->racket_pos) {
            if(ball->y < racket->racket_pos + 100) {
                ball->x = ball->radius + 30;
                if(ball->v_x > 0) ball->v_x += 0.1;
                else ball->v_x -= 0.1;
                ball->v_x = -(ball->v_x);
            }
        }
    }
}

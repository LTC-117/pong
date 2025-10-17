#include "game.h"
#include "ui.h"


//Function for generating the table details
void table_create(SDL_Surface *surface)
{
    //Faixa central:
    SDL_Rect central_limit = (SDL_Rect) {(SCREEN_WIDTH / 2), 0, 1, SCREEN_HEIGHT};
    SDL_FillRect(surface, &central_limit, WHITE);

    //Faixa lateral direita:
    SDL_Rect right_limit = (SDL_Rect) {(SCREEN_WIDTH - 1), 0, 1, SCREEN_HEIGHT};
    SDL_FillRect(surface, &right_limit, WHITE);

    //Faixa superior:
    SDL_Rect upper_limit = (SDL_Rect) {0, 0, SCREEN_WIDTH, 1};
    SDL_FillRect(surface, &upper_limit, WHITE);

    //Faixa inferior :
    SDL_Rect lower_limit = (SDL_Rect) {0, (SCREEN_HEIGHT - 1), SCREEN_WIDTH, 1};
    SDL_FillRect(surface, &lower_limit, WHITE);
}



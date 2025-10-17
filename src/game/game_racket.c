#include "game.h"
#include "ui.h"


// TODO: Implement new features and formats for racket


//Generates the playing bar (racket)
void racket_create(SDL_Surface *surface, Racket *racket)
{
    // Preencher o racket na nova posição
    SDL_Rect real_racket = (SDL_Rect) {0, racket->racket_pos, 30, 100};
    SDL_FillRect(surface, &real_racket, WHITE);
}

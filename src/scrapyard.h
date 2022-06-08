#ifndef SCRAPYARD_H
#define SCRAPYARD_H

#include <SDL2/SDL.h>
#include <time.h>
#include "player.h"

enum scrapyard_state{
    SCRAPYARD_OPEN,
    SCRAPYARD_CLOSING,
    SCRAPYARD_CLOSED
};

struct scrapyard_t{
    SDL_Rect rect;
    enum scrapyard_state state;
    time_t last_state_change;
};

void scrapyards_init(SDL_Renderer *r);
void scrapyards_update(player_t* p);
void scrapyards_draw(SDL_Renderer* r);

#endif

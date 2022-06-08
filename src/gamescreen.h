#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SDL2/SDL.h>

void game_start(SDL_Renderer* renderer);
char game_update(void);
void game_draw(void);
void game_add_score(uint x);
void game_change_weight(uint x);

#endif

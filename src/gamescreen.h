#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void game_start(SDL_Renderer* renderer);
char game_update(void);
void game_draw(void);
void game_add_score(uint x);
void game_change_weight(uint x);
void game_update_health_text(void);

extern Mix_Chunk* snd_explode;
extern Mix_Chunk* snd_oof;
extern Mix_Chunk* snd_pickup;
extern Mix_Chunk* snd_dropoff;

#endif

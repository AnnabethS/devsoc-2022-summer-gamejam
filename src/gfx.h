#ifndef GFX_H
#define GFX_H

#include <SDL2/SDL_ttf.h>

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900

extern TTF_Font* main_font;
extern SDL_Colour colour_white;

void gfx_load(void);

#endif

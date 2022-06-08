#include "gfx.h"

TTF_Font* main_font;
SDL_Color colour_white = {255, 255, 255, 255};

void gfx_load(void)
{
    main_font = TTF_OpenFont("res/font.ttf", 32);
    if(main_font == NULL)
        fprintf(stderr, "could not load font\n");
}

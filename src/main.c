#include "asteroid.h"
#include "gamescreen.h"
#include "sdl_util.h"
#include "anna-layer.h"
#include "player.h"
#include "gfx.h"
#include "scrapyard.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#define TITLETEXT "gamejam"

#define TICK_INTERVAL 15

#define BG_STAR_AMOUNT 30
#define BG_STAR_SIZE 2

enum game_state{GAME_GAMESCREEN, GAME_MENU, GAME_LOSS};

static u32 next_tick;

u32 timeLeft()
{
	u32 now = SDL_GetTicks();
	if(next_tick <= now)
		return 0;
	else
		return next_tick - now;
}

int main()
{
    enum game_state state = GAME_GAMESCREEN;
	SDL_Window* window;
	SDL_Renderer* renderer;
	basicSetup(SCREENWIDTH, SCREENHEIGHT,
			   (SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO),
			   TITLETEXT, &window, &renderer);

	
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

    TTF_Init();

    gfx_load();

    game_start(renderer);

	char running = 1;
	while(running)
	{
        switch(state)
        {
        case GAME_GAMESCREEN:
            running = game_update();
            game_draw();
            break;
        case GAME_LOSS:
        case GAME_MENU:
            running = 0;
            break;
        }

        // delay until the next frame
        SDL_Delay(timeLeft());
        next_tick += TICK_INTERVAL;
	}

}

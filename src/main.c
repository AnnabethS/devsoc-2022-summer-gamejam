#include "sdl_util.h"
#include "anna-layer.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900
#define TITLETEXT "gamejam"

#define TICK_INTERVAL 15

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
	SDL_Window* window;
	SDL_Renderer* renderer;
	basicSetup(SCREENWIDTH, SCREENHEIGHT,
			   (SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER),
			   TITLETEXT, &window, &renderer);

	
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

    thrust_state accelerate = THRUST_NONE;
    turn_state turn = TURN_NONE;
    
    player_t player;
    player_init(&player, renderer);

	char running = 1;
	while(running)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					running = 0;
					break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    accelerate = THRUST_ACCELERATE;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    accelerate = THRUST_REVERSE;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    turn = TURN_RIGHT;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    turn = TURN_LEFT;
                    break;
				default:
					break;
				}
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.scancode)
                {
				case SDL_SCANCODE_ESCAPE:
					running = 0;
					break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if(accelerate == THRUST_ACCELERATE) accelerate = THRUST_NONE;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(accelerate == THRUST_REVERSE) accelerate = THRUST_NONE;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    if(turn == TURN_RIGHT) turn = TURN_NONE;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    if(turn == TURN_LEFT) turn = TURN_NONE;
                    break;
				default:
					break;
                }
			}
		}

        player_update(&player, accelerate, turn);

		// clear the back buffer
		SDL_RenderClear(renderer); 

		// copy texture to back buffer
        player_draw(&player, renderer);

		// swap front and back buffers
		SDL_RenderPresent(renderer);

		// delay until the next frame
		SDL_Delay(timeLeft());
		next_tick += TICK_INTERVAL;
	}
}

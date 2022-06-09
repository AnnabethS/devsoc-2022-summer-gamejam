#include "gamescreen.h"
#include "asteroid.h"
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
#include <SDL2/SDL_mixer.h>

#define BG_STAR_AMOUNT 30
#define BG_STAR_SIZE 2

static player_t player;
static thrust_state accelerate = THRUST_NONE;
static turn_state turn = TURN_NONE;
static SDL_Rect bg_stars[BG_STAR_AMOUNT] = {0};
static SDL_Texture* score_text_texture;
static SDL_Rect score_text_rect = {0};
static SDL_Texture* weight_text_texture;
static SDL_Rect weight_text_rect = {0};
static SDL_Texture* hp_text_texture;
static SDL_Rect hp_text_rect = {0};
static SDL_Renderer* r;

Mix_Chunk* snd_explode;
Mix_Chunk* snd_oof;
Mix_Chunk* snd_pickup;
Mix_Chunk* snd_dropoff;

uint score = 0;

void game_start(SDL_Renderer* renderer)
{
    r = renderer;
    accelerate = THRUST_NONE;
    turn = TURN_NONE;

    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 1024);
    snd_explode = Mix_LoadWAV("res/explosion.wav");
    snd_oof = Mix_LoadWAV("res/oof.wav");
    snd_pickup = Mix_LoadWAV("res/pickup.wav");
    snd_dropoff = Mix_LoadWAV("res/deliver.wav");
    
    player_init(&player, r);

    asteroid_load_textures(r);

    scrapyards_init(r);

    srand(time(NULL));
    
    for(int i=0; i < BG_STAR_AMOUNT; i++)
    {
        bg_stars[i].x = rand() % SCREENWIDTH;
        bg_stars[i].y = rand() % SCREENHEIGHT;
        bg_stars[i].w = BG_STAR_SIZE;
        bg_stars[i].h = BG_STAR_SIZE;
    }
    game_add_score(0);
    game_change_weight(0);
    game_update_health_text();
}

char game_update(void)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            return 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                return 0;
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
                return 0;
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
    asteroid_all_update(&player);
    scrapyards_update(&player);

    if(player.health <= 0)
        return 0;
    else
        return 1;
}

void game_draw(void)
{
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    // clear the back buffer
    SDL_RenderClear(r); 

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    for(int i=0; i < BG_STAR_AMOUNT; i++)
    {
        SDL_RenderFillRect(r, bg_stars+i);
    }

    asteroid_all_draw(r);

    scrapyards_draw(r);

    // copy texture to back buffer
    player_draw(&player, r);

    SDL_RenderCopy(r, score_text_texture, NULL, &score_text_rect);
    SDL_RenderCopy(r, weight_text_texture, NULL, &weight_text_rect);
    SDL_RenderCopy(r, hp_text_texture, NULL, &hp_text_rect);

    // swap front and back buffers
    SDL_RenderPresent(r);
}

void game_add_score(uint x)
{
    score += x;
    char newstr[30] = {0};
    sprintf(newstr, "Sold Scrap: %d", score);
    SDL_Surface* s = TTF_RenderText_Solid(main_font, newstr, colour_white);
    score_text_texture = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    SDL_QueryTexture(score_text_texture, NULL, NULL,
                     &score_text_rect.w, &score_text_rect.h);
    score_text_rect.y = score_text_rect.h;
    score_text_rect.x = (SCREENWIDTH / 2) - (score_text_rect.w / 2);
}

void game_change_weight(uint x)
{
    char newstr[30] = {0};
    sprintf(newstr, "Current Ship Weight: %d", x);
    SDL_Surface* s = TTF_RenderText_Solid(main_font, newstr, colour_white);
    weight_text_texture = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    SDL_QueryTexture(weight_text_texture, NULL, NULL,
                     &weight_text_rect.w, &weight_text_rect.h);
    weight_text_rect.y = weight_text_rect.h * 2;
    weight_text_rect.x = (SCREENWIDTH / 2) - (weight_text_rect.w / 2);
}

void game_update_health_text(void)
{
    char newstr[30] = {0};
    sprintf(newstr, "Hull Integrity: %d/%d", player.health, player.max_health);
    SDL_Surface* s = TTF_RenderText_Solid(main_font, newstr, colour_white);
    hp_text_texture = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    SDL_QueryTexture(hp_text_texture, NULL, NULL,
                     &hp_text_rect.w, &hp_text_rect.h);
    hp_text_rect.y = hp_text_rect.h * 3;
    hp_text_rect.x = (SCREENWIDTH / 2) - (hp_text_rect.w / 2);
}

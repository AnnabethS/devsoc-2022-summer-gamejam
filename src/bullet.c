#include "bullet.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#define MAX_BULLETS 50
#define BULLET_SPEED 10
#define TRAIL_LENGTH 20

bullet_t* bullets[MAX_BULLETS];

void bullet_init(vec2f* pos, float angle)
{
    bullet_t* b = NULL;
    for(int i=0; i < MAX_BULLETS; i++)
    {
        if(bullets[i] == NULL)
        {
            bullets[i] = malloc(sizeof(bullet_t));
            b = bullets[i];
            break;
        }
    }
    if(b == NULL)
    {
        fprintf(stderr, "too many bullets added to static list (anna is lazy)\n");
        return;
    }
    memcpy(&b->position, pos, sizeof(vec2f));
    memcpy(&b->initial_position, pos, sizeof(vec2f));

    printf("bullet added at %.2f %.2f\n", b->initial_position.x, b->initial_position.y);

    vec2fNormalisedVectorFromAngleDegrees(&b->velocity, angle);
    vec2fScalarProduct(&b->velocity, &b->velocity, BULLET_SPEED);
}

void bullet_update(void)
{
    for (int i=0; i < MAX_BULLETS; i++)
    {
        if (bullets[i] != NULL)
        {
            vec2fAdd(&bullets[i]->position, &bullets[i]->position, &bullets[i]->velocity);
        }
    }
}

void bullet_draw(SDL_Renderer* r)
{
    for(int i=0; i < MAX_BULLETS; i++)
    {
        if(bullets[i] != NULL)
        {
            vec2f head;
            vec2f tail;
            head.x = bullets[i]->position.x;
            head.y = bullets[i]->position.y;
            tail.x = bullets[i]->initial_position.x;
            tail.y = bullets[i]->initial_position.y;

            if(vec2fDist(&head, &tail) < TRAIL_LENGTH)
            {
                SDL_RenderDrawLine(r, head.x, head.y, tail.x, tail.y);
                printf("1: %.2f %.2f %.2f %.2f\n", head.x, head.y, tail.x, tail.y);
            }
            else
            {
                vec2f newtail;
                vec2fDiff(&newtail, &head, &tail);
                vec2fClampMagnitude(&newtail, &newtail, TRAIL_LENGTH);
                SDL_RenderDrawLine(r, head.x, head.y, newtail.x, newtail.y);
                printf("1: %.2f %.2f %.2f %.2f\n", head.x, head.y, newtail.x, newtail.y);
            }
        }
    }
}

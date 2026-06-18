#include "app_init.h"
#include "gui.h"
#include "path.h"
#include <stdlib.h>
#include <stdio.h>

AppContext* app_init_sdl(int width, int height, const char* title)
{
    AppContext* ctx = (AppContext*)malloc(sizeof(AppContext));
    if (!ctx) return NULL;

    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        SDL_Log("Erreur > %s\n", SDL_GetError());
        free(ctx);
        return NULL;
    }

    // Initialize TTF subsystem
    clean_init();

    // Create window
    ctx->window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height, 0);

    if (!ctx->window) {
        SDL_Log("Erreur : %s\n", SDL_GetError());
        free(ctx);
        return NULL;
    }

    // Create renderer
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_SOFTWARE);
    if (!ctx->renderer) {
        SDL_Log("Erreur : %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->window);
        free(ctx);
        return NULL;
    }

    // Set background color
    SDL_SetRenderDrawColor(ctx->renderer, 60, 62, 68, 255);
    SDL_RenderClear(ctx->renderer);
    SDL_RenderPresent(ctx->renderer);

    return ctx;
}

void app_cleanup_sdl(AppContext* ctx)
{
    if (!ctx) return;

    cleanResources(ctx->window, ctx->renderer, NULL);
    TTF_Quit();
    free(ctx);
}

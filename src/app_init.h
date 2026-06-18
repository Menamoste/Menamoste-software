#ifndef APP_INIT_H
#define APP_INIT_H

#include <SDL2/SDL.h>

// SDL initialization and window setup
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} AppContext;

// Initialize SDL, create window and renderer
AppContext* app_init_sdl(int width, int height, const char* title);

// Cleanup SDL resources
void app_cleanup_sdl(AppContext* ctx);

#endif

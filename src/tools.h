#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>

void setpixel(SDL_Surface *sur, int x, int y, Uint32 pixel);
Uint32 getpixel(SDL_Surface *sur, int x, int y);

#endif

#ifndef SDL2_TTF_COMPAT_H
#define SDL2_TTF_COMPAT_H

#ifdef _WIN32
// Windows scoop package structure
#include <SDL2_ttf/SDL_ttf.h>
#else
// Standard Linux/Mac structure
#include <SDL2/SDL_ttf.h>
#endif

#endif

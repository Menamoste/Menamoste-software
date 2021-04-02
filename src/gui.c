#include <SDL2/SDL.h>

void set_rect(SDL_Rect *rect, int x, int y, int h, int w)
{
    rect->x = x;
    rect->y = y;
    rect->h = h;
    rect->w = w;
}

void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect); 
    SDL_RenderPresent(renderer);
}

void set_color(SDL_Color *colors, size_t i, unsigned char r, unsigned char
g, unsigned char b, unsigned char a)
{
	colors[i].r = r;
	colors[i].g = g;
	colors[i].b = b;
	colors[i].a = a;
} 

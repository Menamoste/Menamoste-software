#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include "gui.h"

const size_t window_width = 1920;
const size_t window_height = 1080;


void rectangle(SDL_Rect rect, SDL_Renderer *renderer, int x, int y, int h, int w)
{
    rect.x = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;
    
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &rect); 
    SDL_RenderPresent(renderer);
}


/*void get_color(SDL_Color *tab)
{
    SDL_Color t[9];
    t[0] = {255, 0, 0, 255};
    t[1] = {0, 255, 0, 255};
    t[2] = {0, 0, 255, 255};
    t[3] = {255, 0, 255, 255};
    t[4] = {0, 255, 255, 255};
    t[5] = {255, 255, 0, 255};
    t[6] = {255, 255, 255, 255};
    t[7] = {0, 0, 0, 255};
    t[8] = {127, 127, 255, 255};
    
} */

int main()
{
    SDL_Window *gui_window;
    SDL_Renderer *renderer;
    SDL_Event event;
 
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        printf("Could not initialize SDL");
        return -1;
    }
 
    gui_window = SDL_CreateWindow("Menamoste GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_FULLSCREEN);
 
    renderer=SDL_CreateRenderer(gui_window,-1,SDL_RENDERER_SOFTWARE);
 
    SDL_SetRenderDrawColor(renderer,255,255,255,0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
 //-----------------------------------------------------------------  Set the big grey rectangle
    
    
    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
    SDL_Rect r = {0, 0, 2100, 400};
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer); 
// ----------------------------------------------------------------- Set all the other rectangles
    SDL_Rect rect[9];
    
    int x = 1100;
    for(int i  = 0; i < 9; i++)
    {
        rectangle(rect[i], renderer, x, 100, 50, 50);
        x += 100;
        
    }
    
    int y = 300;
    for(int i  = 0; i < 4; i++)
    {
        rectangle(rect[i], renderer, y, 100, 50, 50);
        y += 50;     
    }
    
    
  // ----------------------------------------------------------------  Event managing
    while(1)
    {
        SDL_PollEvent(&event);
        if(event.window.event==SDL_WINDOWEVENT_CLOSE)
            break;
        if(event.type==SDL_KEYDOWN)
        {
            if(event.key.keysym.sym==SDLK_ESCAPE)
                break;
        }
 
    }
 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gui_window);
    SDL_Quit();
    
    return 0;
}
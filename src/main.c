#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>

int main () {
	printf("It's the beginning !\n");
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		err(1, "Could not launch the SDL.");
	}
	SDL_Rect rect = {0, 0, 512, 512};
	SDL_Window *window = SDL_CreateWindow("Affichage", 
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0);
	if (!window) {
		err(1, "Could not create a window, bitch");
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
	SDL_RENDERER_SOFTWARE);
	if (!renderer) {
		err(1, "Could not create a renderer, bitch");
	}
	SDL_Surface *surface = SDL_LoadBMP("../res/Lenna.bmp");
	if (!surface) {
		err(1, "Could not create a surface, bitch");
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		err(1, "Could not create a texture, bitch");
	}
	SDL_FreeSurface(surface);
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
	return 0;	
}

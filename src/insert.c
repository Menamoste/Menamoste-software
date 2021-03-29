#include "insert.h"

void insert(SDL_Surface *sur)
{
	char *c = "";
	scanf("%s", c);
	SDL_Surface *my_image = NULL;
	my_image = SDL_LoadBMP(c);
	if(my_image == NULL)
		err(1, "Can't open %s\n", c);
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(my_image, NULL, sur, &position);
	SDL_Flip(sur);
}

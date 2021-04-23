#include "color.h"

//Color a pixel with the appropriate triplet color
void color_pixel(matrix_pack *mat_pack, SDL_Surface *sur, triplet trip, 
int x, int y)
{
    int radius = 30;
    int width = sur->w;
    int height = sur->h;
    unsigned char new_r = trip.r * 255;
    unsigned char new_g = trip.g * 255;
    unsigned char new_b = trip.b * 255;
    Uint32 pixel = SDL_MapRGB(sur->format, new_r, new_g, new_b);
    for (int i = x - radius; i < x + radius; i++) 
    {
	for (int j = y - radius; j < y + radius; j++)
	{
	    if (i < width && i >= 0 && j < height && j >= 0) 
	    {
    	    	set_pixel(sur, i, j, pixel);
    	    	mat_pack_set(mat_pack, j, i, trip);	
            }
	}
    }
}

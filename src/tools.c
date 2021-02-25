#include "tools.h"
//TO REMOVE LATER
#include <stdlib.h>

void set_pixel(SDL_Surface *sur, int x, int y, Uint32 pixel)
{
    int nbOctetsPerPixel = sur->format->BytesPerPixel;
    //p is the address of the pixel we want to modify.
    Uint8 *p = (Uint8 *)sur->pixels + y * sur->pitch + x * nbOctetsPerPixel;
    switch(nbOctetsPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

Uint32 get_pixel(SDL_Surface *sur, int x, int y)
{
    int bpp = sur->format->BytesPerPixel;
    //Here p is the address to the pixel we want to retrieve.
    Uint8 *p = (Uint8 *)sur->pixels + y * sur->pitch + x * bpp;
    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

            case 4:
                return *(Uint32 *)p;
                break;

            default:
                return 0;
    }
}

matrix_pack *sur_to_mat_pack(SDL_Surface *sur) {
	int width  = sur->w;
	int height = sur->h;

	matrix_pack *mat_pack = mat_pack_zero(height, width);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Uint8 r, g, b;
			Uint32 pixel = get_pixel(sur, i, j);
			SDL_GetRGB(pixel, sur->format, &r, &g, &b);

			matrix_set(mat_pack->r, j, i, r);
			matrix_set(mat_pack->g, j, i, g);
			matrix_set(mat_pack->b, j, i, b);
		}
	}

	return mat_pack;
}


#include "color.h"

//Color a pixel in red
void color_pixel(SDL_Surface *sur, int x, int y)
{
    matrix_pack *mat_pack = sur_to_mat_pack(sur);
    unsigned char new_r = 255;
    unsigned char new_g = 0;
    unsigned char new_b = 0;
    Uint32 pixel = SDL_MapRGB(sur->format, new_r, new_g, new_b);
    set_pixel(sur, x, y, pixel);
    triplet trip = {new_r, new_g, new_b};
    mat_pack_set(mat_pack, y, x, trip);
}

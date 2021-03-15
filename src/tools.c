#include "tools.h"

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
			float new_r = r / 255.f;
			float new_g = g / 255.f;
			float new_b = b / 255.f;

			triplet trip = {new_r, new_g, new_b};
			mat_pack_set(mat_pack, j, i, trip);
		}
	}
	return mat_pack;
}

void mat_pack_to_sur(SDL_Surface *sur, matrix_pack *mat_pack) {
	size_t width  = sur->w;
	size_t height = sur->h;
	Uint32 pixel  = 0;

	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			triplet trip = mat_pack_get(mat_pack, j, i);
			unsigned char new_r = trip.r * 255;
			unsigned char new_g = trip.g * 255;
			unsigned char new_b = trip.b * 255;
			pixel = SDL_MapRGB(sur->format, new_r, new_g, new_b);
			set_pixel(sur, i, j, pixel);
		}
	}
}

matrix_pack *rotation(matrix_pack *mat_pack, unsigned char angle) {
	//Data
	double teta = (2 * 3.141559 * angle) / 360.0;
	double costeta = cos(-teta);
	double sinteta = sin(-teta);
	//Take the rows and cols from the mat_pack.
	int rows = (int)mat_pack->r->rows;
	int cols = (int)mat_pack->r->cols;
	//Coordinates of where it rotates.
	int x0 = cols / 2;
	int y0 = rows / 2;
	//Offsets of the coordinates.
	int xoff = 0;
	int yoff = 0;
	//Coordinates of the new points.
	int x2 = 0;
	int y2 = 0;
	//The new matrix.
	matrix_pack *result = mat_pack_zero(rows, cols);
	mat_pack_add(result, 255);
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			x2 = (int)(xoff * costeta - yoff * sinteta + x0);
			y2 = (int)(xoff * sinteta + yoff * costeta + y0);
			if (x2 >= 0 && y2 >= 0 && x2 < cols && y2 < rows) {
				triplet trip = mat_pack_get(mat_pack, y, x);
				mat_pack_set(result, y2, x2, trip);
			}
		}
	}
	return result;
}

//A revoir car matrices of unsigned.
/*matrix_pack *three_shears(matrix_pack *mat_pack, unsigned char angle) {
	
	//Data
	double teta = (2 * 3.141559 * angle) / 360.0;
	double tan_teta_over_2 = tan(teta) / 2;
	double sin_teta = sin(-teta);
	//Take the rows and cols from the mat_pack.
	int rows = (int)mat_pack->r->rows;
	int cols = (int)mat_pack->r->cols;
	//Matrices for calculation.
	matrix *mat1 = matrix_zero(2, 2);
	matrix *mat2 = matrix_zero(2, 2);
	//Put the correct values on the matrices.
	matrix_get(mat1, 0, 0, 1);
	matrix_get(mat1, 1, 0, 1);
	matrix_get(mat1, 0, 1, 1);
	matrix_get(mat1, 1, 1, 1);

	
	//The new matrix.
	matrix_pack *result = mat_pack_zero(rows, cols);
	mat_pack_add(result, 255);
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			if (x2 >= 0 && y2 >= 0 && x2 < cols && y2 < rows) {
				triplet trip = mat_pack_get(mat_pack, y, x);
				mat_pack_set(result, y2, x2, trip);
			}
		}
	}
	return result;
}*/

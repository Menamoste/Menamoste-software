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


//Turns the image on a teta angle
matrix_pack *rotation(matrix_pack *mat_pack, unsigned char angle) {
	double teta = (2 * 3.141559 * angle) / 360.0;
	double costeta = cos(-teta);
	double sinteta = sin(-teta);
	int rows = (int)mat_pack->r->rows;
	int cols = (int)mat_pack->r->cols;
	int x0 = cols / 2;
	int y0 = rows / 2;

	matrix_pack *result = mat_pack_zero(rows, cols);
	for (int x = 0; x < cols; x++) {
		for (int y = 0; y < rows; y++) {
			int xoff = x - x0;
			int yoff = y - y0;
			int x2 = (int)(xoff * costeta -yoff * sinteta + x0);
			int y2 = (int)(xoff * sinteta +yoff * costeta + y0);
			if (x2 >= 0 && y2 >= 0 && x2 < cols && y2 < rows) {
				triplet trip = mat_pack_get(mat_pack, x, y);
				mat_pack_set(result, x2, y2, trip);
			}
		}
	}
	return result;
}

//Keeps the value between 0.0 and 1.0.
float prevent_overflow(float value) {
	if (value > 1.0)
		return 1.0;
	if (value < 0.0)
		return 0.0;
	return value;
}
//Return the convolution matrix of matrix with convo
matrix *mat_convolution(matrix *mat, matrix *convo) {
	size_t rows1 = mat->rows;
	size_t rows2 = convo->rows;
	size_t cols1 = mat->cols;
	size_t cols2 = convo->cols;
	matrix *result = matrix_zero(rows1, cols1);
	//Indexes for matrix : i and j, for convo : k and l.
	for (size_t i = 0; i < rows1; i++) {
		for (size_t j = 0; j < cols1; j++) {
			float accu = 0;
			for (size_t k = 0; k < rows2; k++) {
				for (size_t l = 0; l < cols2; l++) {
					size_t x = i + k - (rows2 / 2);
					size_t y = j + l - (cols2 / 2);
					if (x < rows1 && y < cols1 
					 && x > 0 && y > 0) {
						float mat_val = 
						       matrix_get(mat, x,y);
						float conv_val = 
						       matrix_get(convo, k, l);	
						accu += mat_val * conv_val;
					}
				}
			}
			accu = prevent_overflow(accu);
			matrix_set(result, i, j, accu);
		}
	}
	//Edge Handeling
	float mat_val = 0.0f; 

	for (size_t i = 0; i < rows1; i++) {
		mat_val = matrix_get(result, i, 2);
		matrix_set(result, i, 0, mat_val);
		matrix_set(result, i, 1, mat_val);
		matrix_set(result, i, 2, mat_val);

		mat_val = matrix_get(result, i, cols1 - 2);
		matrix_set(result, i, cols1 - 1, mat_val);
		matrix_set(result, i, cols1 - 2, mat_val);
		matrix_set(result, i, cols1 - 3, mat_val);
	}
	for (size_t j = 0; j < cols1; j++) {
		mat_val = matrix_get(result, 2, j);
		matrix_set(result, 0, j, mat_val);
		matrix_set(result, 1, j, mat_val);
		matrix_set(result, 2, j, mat_val);

		mat_val = matrix_get(result, rows1 - 2, j);
		matrix_set(result, rows1 - 1, j, mat_val);
		matrix_set(result, rows1 - 2, j, mat_val);
		matrix_set(result, rows1 - 3, j, mat_val);
	}

	return result;
}

//Same as ma_convolution but for mat_packs.
void convolution(matrix_pack *mat_pack, matrix *convo) {
	mat_pack->r = mat_convolution(mat_pack->r, convo);
	mat_pack->g = mat_convolution(mat_pack->g, convo);
	mat_pack->b = mat_convolution(mat_pack->b, convo);
}

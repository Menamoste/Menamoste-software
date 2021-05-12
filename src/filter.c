#include "filter.h"

//Image en noir et blanc
void Black_and_white(matrix_pack *mat_pack,SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    Uint8 average = (r+g+b)/3;
		    if(average<127)
		    {
			    r = 0;
			    g = 0;
			    b = 0;
		    }
		    else
		    {
			    r = 255;
			    g = 255;
			    b = 255;
		    }
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Image en nuance de gris
void Grayscale(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    Uint8 average = (r+g+b)/3;
		    r = average;
		    g = average;
		    b = average;
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Couleurs inversées par rapport à l'image originale
void Negative(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    r = 255 - r;
		    g = 255 - g;
		    b = 255 - b;
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

Uint8 f(Uint8 c, int n)
{
	if(c+n > 255)
		return 255;
	else if(c+n < 0)
		return 0;
	else
		return c+n;
}

Uint8 f2(Uint8 c, int n)
{
	if(c <= 127)
		return pow(2*c/255, n);
	else
		return 255 - f2(255-c, n);
}

//Effet pêche sur l'image
void Peach(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    r = f(r, 50);
		    g = f(g, 0);
		    b = f(b, -50);
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Eclaircissement de l'image
void Lighten(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    r = f(r, 20);
		    g = f(g, 60);
		    b = f(b, 0);
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Effet vintage sur l'image
void Vintage(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    r = f(r, 0);
		    g = f(g, 120);
		    b = f(b, 20);
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Assombrissement de l'image 
void Darken(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    r = f(r, -100);
		    g = f(g, 0);
		    b = f(b, 0);
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Contraste de l'image
//Les pixels lumineux (respectivement sombres) le sont encore plus
void Contrast(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = get_pixel(img,j,i);
		    Uint8 r,g,b;
		    SDL_GetRGB(pixel,img->format,&r,&g,&b);
		    r = f2(r, -50);
		    g = f2(g, -50);
		    b = f2(b, -50);
		    Uint32 pixel1 = SDL_MapRGB(img->format,r,g,b);
		    set_pixel(img,j,i,pixel1);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

Uint32 moyenne(SDL_Surface *img, int i, int j, int n)
{
	int initial_h = fmax(i-n,0);
	int initial_w = fmax(j-n,0);
	int final_h = fmin(i+n, img->h-1);
	int final_w = fmin(j+n, img->w-1);
	int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));
	Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
	for(i = initial_h; i < final_h; i++)
		for(j = initial_w; j < final_w; j++)
		{
			Uint32 pixel = get_pixel(img, j, i);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			sum_r += r;
			sum_g += g;
			sum_b += b;
		}
	return SDL_MapRGB(img->format, sum_r/nb_pixel, sum_g/nb_pixel, sum_b/nb_pixel);
}

//Floute l'image
void Blur(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 pixel = moyenne(img, i, j, 9);
		    set_pixel(img,j,i,pixel);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

//Accentue les contours des figures de l'image
void Outline(matrix_pack *mat_pack, SDL_Surface *img, triplet trip)
{
    int x = img->w;
    int y = img->h;
    int i;
    int j;
    for(i=0;i<y;i++)
    {
	    for(j=0;j<x;j++)
	    {
		    Uint32 p = get_pixel(img, j, i);
		    Uint32 pixel = 255 - (p - moyenne(img, i, j, 9));
		    set_pixel(img,j,i,pixel);
		    mat_pack_set(mat_pack, j, i, trip);
	    }
    }
}

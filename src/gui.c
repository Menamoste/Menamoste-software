#include <SDL2/SDL.h>
#include "tools.h"
#include "matrix.h"
#include "resize.h"

extern const size_t window_width;
extern const size_t window_height;
extern const size_t nb_rects;
extern const size_t nb_icons;
extern const size_t icon_size;

void set_color(SDL_Color *colors, size_t i, unsigned char r, unsigned char
g, unsigned char b, unsigned char a)
{
	colors[i].r = r;
	colors[i].g = g;
	colors[i].b = b;
	colors[i].a = a;
}

void set_rect(SDL_Rect *rect, int x, int y, int h, int w)
{
    rect->x = x;
    rect->y = y;
    rect->h = h;
    rect->w = w;
}

void place_rects(SDL_Rect *rects) 
{
	//Icons images placement
        size_t x = 50;
	size_t y = 100;
    	for (size_t i = 0; i < nb_icons; i++)
    	{
		set_rect(&rects[i], x, y, icon_size, icon_size);
        	x += 85;     
    	}
        x += 150;

        //Icons colors placement
    	for (size_t i = nb_icons; i < nb_rects; i++)
    	{
		set_rect(&rects[i], x, y, icon_size, icon_size);
        	x += 100;
    	}
}

void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect); 
}

void load_rects(SDL_Renderer *renderer, SDL_Rect *rects) 
{
	//Load the icons on surfaces.
	SDL_Surface *pencil_surface = SDL_LoadBMP("../res/Icons/pensil.bmp");
	SDL_Surface *cursor_surface = SDL_LoadBMP("../res/Icons/cursor.bmp");
	SDL_Surface *eraser_surface = SDL_LoadBMP("../res/Icons/eraser.bmp");
	SDL_Surface *bucket_surface = SDL_LoadBMP("../res/Icons/bucket.bmp");
	SDL_Surface *filter_surface = SDL_LoadBMP("../res/Icons/filter.bmp");
	SDL_Surface *group_surface  = SDL_LoadBMP("../res/Icons/group.bmp" );
	SDL_Surface *resize_surface = SDL_LoadBMP("../res/Icons/resize.bmp");
	SDL_Surface *rotate_surface = SDL_LoadBMP("../res/Icons/rotate.bmp");

	//Then put them in textures.
	SDL_Texture *pencil_texture = SDL_CreateTextureFromSurface(renderer,
	pencil_surface);
	SDL_Texture *cursor_texture = SDL_CreateTextureFromSurface(renderer,
	cursor_surface);
	SDL_Texture *eraser_texture = SDL_CreateTextureFromSurface(renderer,
	eraser_surface);
	SDL_Texture *bucket_texture = SDL_CreateTextureFromSurface(renderer,
	bucket_surface);
	SDL_Texture *filter_texture = SDL_CreateTextureFromSurface(renderer,
	filter_surface);
	SDL_Texture *group_texture  = SDL_CreateTextureFromSurface(renderer,
	group_surface);
        SDL_Texture *resize_texture = SDL_CreateTextureFromSurface(renderer,
	resize_surface);
	SDL_Texture *rotate_texture = SDL_CreateTextureFromSurface(renderer,
	rotate_surface);

	//Free the surfaces.
	SDL_FreeSurface(pencil_surface);
	SDL_FreeSurface(cursor_surface);
	SDL_FreeSurface(eraser_surface);
	SDL_FreeSurface(bucket_surface);
	SDL_FreeSurface(filter_surface);
	SDL_FreeSurface(group_surface) ;
	SDL_FreeSurface(resize_surface);
	SDL_FreeSurface(rotate_surface);

	//And finally copy them where they belong.
	SDL_RenderCopy(renderer, pencil_texture, NULL, &rects[0]);
	SDL_RenderCopy(renderer, cursor_texture, NULL, &rects[1]);
	SDL_RenderCopy(renderer, eraser_texture, NULL, &rects[2]);
	SDL_RenderCopy(renderer, bucket_texture, NULL, &rects[3]);
	SDL_RenderCopy(renderer, filter_texture, NULL, &rects[4]);
	SDL_RenderCopy(renderer, group_texture , NULL, &rects[5]);
	SDL_RenderCopy(renderer, resize_texture, NULL, &rects[6]);
	SDL_RenderCopy(renderer, rotate_texture, NULL, &rects[7]);
} 

void cleanResources(SDL_Window *window, SDL_Renderer *renderer,
 SDL_Texture *texture) 
{
	if (texture  != NULL)
		SDL_DestroyTexture(texture);
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (window   != NULL)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

matrix_pack *modify_image(matrix_pack *mat_pack, matrix *convo, int mode) 
{
	//Mode filter
	if (mode == 1) 
	{
		convolution(mat_pack, convo);
		return mat_pack;
	}

	//Mode resize
	if (mode == 2) 
	{
		size_t new_w = 768;
		size_t new_y = 145;
		matrix_pack *mat_pack2 = resize(mat_pack, new_w, new_y);
		mat_pack_free(mat_pack);
		return mat_pack2;
	}

	//Mode rotate
	if (mode == 3) 
	{
		matrix_pack *mat_pack2 = rotation(mat_pack, 45);
		return mat_pack2;
	}
	return NULL;
}

void print_image(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Surface *sur,
 matrix_pack *mat_pack) 
{
	//Put the matrix on the surface.
	mat_pack_to_sur(sur, mat_pack);

	//Put the surface in a texture.
	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer,
	sur);
	if (!image_texture) {
		SDL_Log("Erreur : %s\n", SDL_GetError());
		cleanResources(NULL, renderer, NULL);
	}
	
	//Print the image
	SDL_QueryTexture(image_texture, NULL, NULL, &rect->w, &rect->h);
	SDL_RenderCopy(renderer, image_texture, NULL, rect);

	//Show the result.
	SDL_RenderPresent(renderer);
}

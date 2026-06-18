#include <SDL2/SDL.h>
#include "tools.h"
#include "matrix.h"
#include "resize.h"
#include "sdl2_ttf_compat.h"
#include <stdio.h>

extern const size_t window_width;
extern const size_t window_height;
extern const size_t nb_rects;
extern const size_t nb_icons;
extern const size_t icon_size;

//Global icon texture storage
static SDL_Texture *icon_textures_normal[8] = {NULL};
static SDL_Texture *icon_textures_select[8] = {NULL};

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
	//Icons images placement - positioned in middle to lower area of toolbar
        size_t x = 120;
	size_t y = (330 - icon_size) / 2 + 50;  // Move down from center: 117 + 50 = 167
    	for (size_t i = 0; i < nb_icons; i++)
    	{
		set_rect(&rects[i], x, y, icon_size, icon_size);
        	x += 180;  // Equal spacing for all buttons
    	}

        //Icons colors placement (3x3 grid on right side)
        x = window_width - 225;  // Right side of screen
        y = (330 - 210) / 2 + 50;  // Align with buttons
    	for (size_t i = nb_icons; i < nb_rects; i++)
    	{
    		size_t color_idx = i - nb_icons;
    		size_t row = color_idx / 3;
    		size_t col = color_idx % 3;
    		int color_x = x + (col * 70);
    		int color_y = y + (row * 70);
		set_rect(&rects[i], color_x, color_y, 60, 60);
    	}
}

void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_toolbar_bg(SDL_Renderer *renderer)
{
	//Fill toolbar background - extends lower to cover buttons and labels, below color palette
	SDL_Rect toolbar_rect = {0, 0, window_width, 330};
	SDL_SetRenderDrawColor(renderer, 40, 42, 48, 255);
	SDL_RenderFillRect(renderer, &toolbar_rect);

	//Draw bottom separator line
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);
	SDL_RenderDrawLine(renderer, 0, 329, window_width, 329);
	SDL_RenderDrawLine(renderer, 0, 330, window_width, 330);

	//Draw vertical separator between tool groups
	//Buttons at y = 167, height 96, so span 167-263, separator spans 140-320
	int sep_y_top = 140;
	int sep_y_bottom = 320;

	//Single separator: Between Bucket (3) and Filter (4), in the middle
	int sep_x = 120 + 180 * 3 + 96 + (180 - 96) / 2;  // Middle of gap between buttons 3 and 4
	SDL_RenderDrawLine(renderer, sep_x, sep_y_top, sep_x, sep_y_bottom);
	SDL_RenderDrawLine(renderer, sep_x + 1, sep_y_top, sep_x + 1, sep_y_bottom);
}

void draw_button_border(SDL_Renderer *renderer, SDL_Rect rect)
{
	//Draw 1px dark border 3px outside the rect
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);

	SDL_Rect outer = {rect.x - 3, rect.y - 3, rect.w + 6, rect.h + 6};
	SDL_RenderDrawRect(renderer, &outer);
}

void draw_active_border(SDL_Renderer *renderer, SDL_Rect rect, int active)
{
	if (active == 1) {
		//Draw 2px gold border 3px outside
		SDL_SetRenderDrawColor(renderer, 255, 195, 50, 255);
		SDL_Rect outer = {rect.x - 3, rect.y - 3, rect.w + 6, rect.h + 6};
		SDL_RenderDrawRect(renderer, &outer);
		SDL_Rect outer2 = {rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4};
		SDL_RenderDrawRect(renderer, &outer2);
	} else {
		//Erase gold border by redrawing toolbar bg and dark border
		SDL_SetRenderDrawColor(renderer, 40, 42, 48, 255);
		SDL_Rect erase = {rect.x - 3, rect.y - 3, rect.w + 6, rect.h + 6};
		SDL_RenderFillRect(renderer, &erase);
		draw_button_border(renderer, rect);
	}
}

void load_icon_variants(SDL_Renderer *renderer)
{
	//Icon paths
	const char *paths_normal[] = {
		"res/Icons/pensil.bmp",
		"res/Icons/cursor.bmp",
		"res/Icons/eraser.bmp",
		"res/Icons/bucket.bmp",
		"res/Icons/filter.bmp",
		"res/Icons/group.bmp",
		"res/Icons/resize.bmp",
		"res/Icons/rotate.bmp"
	};
	const char *paths_select[] = {
		"res/Icons/pensil_select.bmp",
		"res/Icons/cursor_select.bmp",
		"res/Icons/eraser_select.bmp",
		"res/Icons/bucket_select.bmp",
		"res/Icons/filter_select.bmp",
		"res/Icons/group_select.bmp",
		"res/Icons/resize_select.bmp",
		"res/Icons/rotate_select.bmp"
	};

	//Load normal icons
	for (size_t i = 0; i < 8; i++) {
		printf("Loading icon: %s\n", paths_normal[i]);
		SDL_Surface *surf = SDL_LoadBMP(paths_normal[i]);
		if (surf) {
			icon_textures_normal[i] = SDL_CreateTextureFromSurface(renderer, surf);
			printf("  ✓ Loaded and created texture\n");
			SDL_FreeSurface(surf);
		} else {
			printf("  ✗ Failed to load: %s\n", SDL_GetError());
		}
	}

	//Load select icons
	for (size_t i = 0; i < 8; i++) {
		printf("Loading icon select: %s\n", paths_select[i]);
		SDL_Surface *surf = SDL_LoadBMP(paths_select[i]);
		if (surf) {
			icon_textures_select[i] = SDL_CreateTextureFromSurface(renderer, surf);
			printf("  ✓ Loaded and created texture\n");
			SDL_FreeSurface(surf);
		} else {
			printf("  ✗ Failed to load: %s\n", SDL_GetError());
		}
	}
}

void render_icon_variant(SDL_Renderer *renderer, SDL_Rect rect, int icon_idx, int variant)
{
	if (icon_idx < 0 || icon_idx >= 8) return;

	SDL_Texture *texture = (variant == 1) ? icon_textures_select[icon_idx] : icon_textures_normal[icon_idx];

	printf("render_icon_variant(%d, variant=%d): texture=%p\n", icon_idx, variant, texture);

	//Clear the rect area with toolbar background color
	SDL_SetRenderDrawColor(renderer, 40, 42, 48, 255);
	SDL_RenderFillRect(renderer, &rect);

	//If texture exists, render it
	if (texture) {
		int ret = SDL_RenderCopy(renderer, texture, NULL, &rect);
		if (ret != 0) {
			printf("  SDL_RenderCopy failed: %s\n", SDL_GetError());
		} else {
			printf("  ✓ Icon rendered\n");
		}
	} else {
		printf("  ✗ Icon texture is NULL\n");
	}
}

void draw_color_swatch(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, int selected)
{
	//Fill with color
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);

	//Draw 1px dark outer border always
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);
	SDL_RenderDrawRect(renderer, &rect);

	//If selected, draw 3px white highlight border
	if (selected) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect highlight1 = {rect.x - 1, rect.y - 1, rect.w + 2, rect.h + 2};
		SDL_RenderDrawRect(renderer, &highlight1);
		SDL_Rect highlight2 = {rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4};
		SDL_RenderDrawRect(renderer, &highlight2);
		SDL_Rect highlight3 = {rect.x - 3, rect.y - 3, rect.w + 6, rect.h + 6};
		SDL_RenderDrawRect(renderer, &highlight3);
	}
}

void draw_canvas_border(SDL_Renderer *renderer, SDL_Rect rect)
{
	//Draw 2px separator border 4px outside the rect
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);

	SDL_Rect outer = {rect.x - 4, rect.y - 4, rect.w + 8, rect.h + 8};
	SDL_RenderDrawRect(renderer, &outer);
	SDL_Rect outer2 = {rect.x - 3, rect.y - 3, rect.w + 6, rect.h + 6};
	SDL_RenderDrawRect(renderer, &outer2);
}

void draw_color_palette(SDL_Renderer *renderer, SDL_Rect *rects, SDL_Color *colors, int active_color_idx)
{
	//Draw all 9 color swatches in 3x3 grid
	for (size_t i = 0; i < 9; i++) {
		size_t rect_idx = nb_icons + i;
		int is_selected = (i == active_color_idx) ? 1 : 0;
		draw_color_swatch(renderer, rects[rect_idx], colors[i], is_selected);
	}
}

void load_rects(SDL_Renderer *renderer, SDL_Rect *rects)
{
	//Load the icons on surfaces.
	SDL_Surface *pencil_surface = SDL_LoadBMP("res/Icons/pensil.bmp");
	SDL_Surface *cursor_surface = SDL_LoadBMP("res/Icons/cursor.bmp");
	SDL_Surface *eraser_surface = SDL_LoadBMP("res/Icons/eraser.bmp");
	SDL_Surface *bucket_surface = SDL_LoadBMP("res/Icons/bucket.bmp");
	SDL_Surface *filter_surface = SDL_LoadBMP("res/Icons/filter.bmp");
	SDL_Surface *group_surface  = SDL_LoadBMP("res/Icons/group.bmp" );
	SDL_Surface *resize_surface = SDL_LoadBMP("res/Icons/resize.bmp");
	SDL_Surface *rotate_surface = SDL_LoadBMP("res/Icons/rotate.bmp");

	//Then put them in textures.
	SDL_Texture *pencil_texture = (pencil_surface) ? SDL_CreateTextureFromSurface(renderer, pencil_surface) : NULL;
	SDL_Texture *cursor_texture = (cursor_surface) ? SDL_CreateTextureFromSurface(renderer, cursor_surface) : NULL;
	SDL_Texture *eraser_texture = (eraser_surface) ? SDL_CreateTextureFromSurface(renderer, eraser_surface) : NULL;
	SDL_Texture *bucket_texture = (bucket_surface) ? SDL_CreateTextureFromSurface(renderer, bucket_surface) : NULL;
	SDL_Texture *filter_texture = (filter_surface) ? SDL_CreateTextureFromSurface(renderer, filter_surface) : NULL;
	SDL_Texture *group_texture  = (group_surface) ? SDL_CreateTextureFromSurface(renderer, group_surface) : NULL;
	SDL_Texture *resize_texture = (resize_surface) ? SDL_CreateTextureFromSurface(renderer, resize_surface) : NULL;
	SDL_Texture *rotate_texture = (rotate_surface) ? SDL_CreateTextureFromSurface(renderer, rotate_surface) : NULL;

	//Free the surfaces.
	if (pencil_surface) SDL_FreeSurface(pencil_surface);
	if (cursor_surface) SDL_FreeSurface(cursor_surface);
	if (eraser_surface) SDL_FreeSurface(eraser_surface);
	if (bucket_surface) SDL_FreeSurface(bucket_surface);
	if (filter_surface) SDL_FreeSurface(filter_surface);
	if (group_surface) SDL_FreeSurface(group_surface);
	if (resize_surface) SDL_FreeSurface(resize_surface);
	if (rotate_surface) SDL_FreeSurface(rotate_surface);

	//And finally copy them where they belong.
	if (pencil_texture) SDL_RenderCopy(renderer, pencil_texture, NULL, &rects[0]);
	if (cursor_texture) SDL_RenderCopy(renderer, cursor_texture, NULL, &rects[1]);
	if (eraser_texture) SDL_RenderCopy(renderer, eraser_texture, NULL, &rects[2]);
	if (bucket_texture) SDL_RenderCopy(renderer, bucket_texture, NULL, &rects[3]);
	if (filter_texture) SDL_RenderCopy(renderer, filter_texture, NULL, &rects[4]);
	if (group_texture) SDL_RenderCopy(renderer, group_texture, NULL, &rects[5]);
	if (resize_texture) SDL_RenderCopy(renderer, resize_texture, NULL, &rects[6]);
	if (rotate_texture) SDL_RenderCopy(renderer, rotate_texture, NULL, &rects[7]);

	//Free textures
	if (pencil_texture) SDL_DestroyTexture(pencil_texture);
	if (cursor_texture) SDL_DestroyTexture(cursor_texture);
	if (eraser_texture) SDL_DestroyTexture(eraser_texture);
	if (bucket_texture) SDL_DestroyTexture(bucket_texture);
	if (filter_texture) SDL_DestroyTexture(filter_texture);
	if (group_texture) SDL_DestroyTexture(group_texture);
	if (resize_texture) SDL_DestroyTexture(resize_texture);
	if (rotate_texture) SDL_DestroyTexture(rotate_texture);
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

matrix_pack *resize_image(matrix_pack *mat_pack, int new_w, int new_y)
{
    	//Mode resize
	matrix_pack *mat_pack2 = resize(mat_pack, new_w, new_y);
	mat_pack_free(mat_pack);
	return mat_pack2;
}

matrix_pack *modify_image(matrix_pack *mat_pack, matrix *convo, int mode) 
{
	//Mode filter
	if (mode == 1) 
	{
		convolution(mat_pack, convo);
		return mat_pack;
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

	//Destroy texture
	SDL_DestroyTexture(image_texture);
}

void draw_top_ribbon(SDL_Renderer *renderer, const char *filename)
{
	//Draw top ribbon background - compact, at top of toolbar
	SDL_SetRenderDrawColor(renderer, 50, 52, 58, 255);
	SDL_Rect ribbon = {0, 5, window_width, 20};
	SDL_RenderFillRect(renderer, &ribbon);

	//Render project name and filename text
	TTF_Font *font = TTF_OpenFont("res/Fonts/arial.ttf", 16);
	if (font && filename) {
		SDL_Color text_color = {180, 180, 180, 255};
		char full_title[256];
		snprintf(full_title, sizeof(full_title), "Menamoste - %s", filename);
		SDL_Surface *text_surf = TTF_RenderText_Blended(font, full_title, text_color);
		if (text_surf) {
			SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
			if (text_tex) {
				SDL_Rect text_rect = {10, 7, text_surf->w, text_surf->h};
				SDL_RenderCopy(renderer, text_tex, NULL, &text_rect);
				SDL_DestroyTexture(text_tex);
			}
			SDL_FreeSurface(text_surf);
		}
		TTF_CloseFont(font);
	}
}

void draw_image_tabs(SDL_Renderer *renderer)
{
	//Draw image tabs/management area - extends to color grid
	SDL_SetRenderDrawColor(renderer, 45, 47, 53, 255);
	SDL_Rect tabs_area = {0, 25, window_width, 85};
	SDL_RenderFillRect(renderer, &tabs_area);

	//Draw default tab background - extends to top of color grid
	SDL_SetRenderDrawColor(renderer, 55, 57, 63, 255);
	SDL_Rect tab = {10, 37, 250, 73};
	SDL_RenderFillRect(renderer, &tab);

	//Draw tab border
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);
	SDL_RenderDrawRect(renderer, &tab);

	//Render tab text "Lenna.bmp" - centered in widget, larger font
	TTF_Font *font = TTF_OpenFont("res/Fonts/arial.ttf", 22);
	if (font) {
		SDL_Color text_color = {200, 200, 200, 255};
		SDL_Surface *text_surf = TTF_RenderText_Blended(font, "Lenna.bmp", text_color);
		if (text_surf) {
			SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
			if (text_tex) {
				int text_x = 10 + (250 - text_surf->w) / 2;  // Center within widget (width 250)
				SDL_Rect text_rect = {text_x, 55, text_surf->w, text_surf->h};
				SDL_RenderCopy(renderer, text_tex, NULL, &text_rect);
				SDL_DestroyTexture(text_tex);
			}
			SDL_FreeSurface(text_surf);
		}
		TTF_CloseFont(font);
	}
}

void draw_status_bar(SDL_Renderer *renderer, int img_width, int img_height, int zoom_percent)
{
	//Draw status bar background at bottom
	SDL_SetRenderDrawColor(renderer, 40, 42, 48, 255);
	SDL_Rect status_bar = {0, window_height - 35, window_width, 35};
	SDL_RenderFillRect(renderer, &status_bar);

	//Draw separator line at top
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);
	SDL_RenderDrawLine(renderer, 0, window_height - 36, window_width, window_height - 36);
	SDL_RenderDrawLine(renderer, 0, window_height - 35, window_width, window_height - 35);

	//Draw zoom slider background (right section)
	SDL_SetRenderDrawColor(renderer, 50, 52, 58, 255);
	SDL_Rect zoom_bg = {window_width - 300, window_height - 32, 280, 18};
	SDL_RenderFillRect(renderer, &zoom_bg);

	//Draw zoom slider border
	SDL_SetRenderDrawColor(renderer, 70, 73, 82, 255);
	SDL_RenderDrawRect(renderer, &zoom_bg);

	//Render image dimensions text (left)
	TTF_Font *font = TTF_OpenFont("res/Fonts/arial.ttf", 18);
	if (font) {
		char dims_text[32];
		if (img_width > 0 && img_height > 0) {
			snprintf(dims_text, sizeof(dims_text), "%d x %d px", img_width, img_height);
		} else {
			snprintf(dims_text, sizeof(dims_text), "-- x -- px");
		}

		SDL_Color text_color = {200, 200, 200, 255};
		SDL_Surface *dims_surf = TTF_RenderText_Blended(font, dims_text, text_color);
		if (dims_surf) {
			SDL_Texture *dims_tex = SDL_CreateTextureFromSurface(renderer, dims_surf);
			if (dims_tex) {
				SDL_Rect dims_rect = {15, window_height - 30, dims_surf->w, dims_surf->h};
				SDL_RenderCopy(renderer, dims_tex, NULL, &dims_rect);
				SDL_DestroyTexture(dims_tex);
			}
			SDL_FreeSurface(dims_surf);
		}

		//Render zoom percentage text (right)
		char zoom_text[16];
		snprintf(zoom_text, sizeof(zoom_text), "%d%%", zoom_percent);
		SDL_Surface *zoom_surf = TTF_RenderText_Blended(font, zoom_text, text_color);
		if (zoom_surf) {
			SDL_Texture *zoom_tex = SDL_CreateTextureFromSurface(renderer, zoom_surf);
			if (zoom_tex) {
				SDL_Rect zoom_rect = {window_width - 70, window_height - 35, zoom_surf->w, zoom_surf->h};
				SDL_RenderCopy(renderer, zoom_tex, NULL, &zoom_rect);
				SDL_DestroyTexture(zoom_tex);
			}
			SDL_FreeSurface(zoom_surf);
		}

		TTF_CloseFont(font);
	}
}

void draw_tool_labels(SDL_Renderer *renderer, SDL_Rect *rects)
{
	const char *tool_names[] = {
		"Pencil",
		"Select",
		"Eraser",
		"Bucket",
		"Filter",
		"Group",
		"Resize",
		"Rotate"
	};

	TTF_Font *font = TTF_OpenFont("res/Fonts/arial.ttf", 14);
	if (!font) return;

	SDL_Color text_color = {200, 200, 200, 255};

	for (size_t i = 0; i < nb_icons; i++) {
		SDL_Surface *text_surf = TTF_RenderText_Blended(font, tool_names[i], text_color);
		if (text_surf) {
			SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
			if (text_tex) {
				//Position text below the icon, centered
				int text_x = rects[i].x + (icon_size - text_surf->w) / 2;
				int text_y = rects[i].y + icon_size + 15;
				SDL_Rect text_rect = {text_x, text_y, text_surf->w, text_surf->h};
				SDL_RenderCopy(renderer, text_tex, NULL, &text_rect);
				SDL_DestroyTexture(text_tex);
			}
			SDL_FreeSurface(text_surf);
		}
	}

	TTF_CloseFont(font);
}

void draw_ui_frame(SDL_Renderer *renderer, const char *filename, int img_width, int img_height, int zoom_percent)
{
	//Clear with background color (gray below canvas area)
	SDL_SetRenderDrawColor(renderer, 60, 62, 68, 255);
	SDL_Rect bg = {0, 330, window_width, window_height - 365};
	SDL_RenderFillRect(renderer, &bg);

	//Draw all UI elements
	draw_toolbar_bg(renderer);
	draw_top_ribbon(renderer, filename);
	draw_image_tabs(renderer);
	draw_status_bar(renderer, img_width, img_height, zoom_percent);
}

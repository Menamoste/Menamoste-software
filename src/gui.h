#include <stdio.h>
#include <SDL2/SDL.h>
#include "matrix.h"

#ifndef GUI_H
#define GUI_H

//Set a color on a SDL_Color array.
void set_color(SDL_Color *colors, size_t i, unsigned char r, unsigned char
g, unsigned char b, unsigned char a);

//Set the propries on a rectangle.
void set_rect(SDL_Rect *rect, int x, int y, int h, int w);

//Place the correct coordinates on the rects structure.
void place_rects(SDL_Rect *rects);

//Draw a rectangle on the renderer.
void draw_rect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color);

//Load the icons on the renderer.
void load_rects(SDL_Renderer *renderer, SDL_Rect *rects);

//Clean the ressources to avoid memory leaks.
void cleanResources(SDL_Window *window, SDL_Renderer *renderer, 
SDL_Texture *texture);

//Resize the image.
matrix_pack *resize_image(matrix_pack *mat_pack, int new_w, int new_y);

//Modify the image according to the mode chosen.
matrix_pack *modify_image(matrix_pack *mat_pack, matrix *convo, int mode);

//Print the image on the renderer
void print_image(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Surface *sur,
 matrix_pack *mat_pack);

//Draw toolbar background
void draw_toolbar_bg(SDL_Renderer *renderer);

//Draw button border
void draw_button_border(SDL_Renderer *renderer, SDL_Rect rect);

//Draw active tool border (gold) or inactive
void draw_active_border(SDL_Renderer *renderer, SDL_Rect rect, int active);

//Load all icon variants (normal and select)
void load_icon_variants(SDL_Renderer *renderer);

//Render icon variant (0=normal, 1=select)
void render_icon_variant(SDL_Renderer *renderer, SDL_Rect rect, int icon_idx, int variant);

//Draw color swatch with optional selection highlight
void draw_color_swatch(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, int selected);

//Draw canvas border
void draw_canvas_border(SDL_Renderer *renderer, SDL_Rect rect);

//Draw color palette (3x3 grid)
void draw_color_palette(SDL_Renderer *renderer, SDL_Rect *rects, SDL_Color *colors, int active_color_idx);

//Draw top ribbon with filename
void draw_top_ribbon(SDL_Renderer *renderer, const char *filename);

//Draw image tabs/management area
void draw_image_tabs(SDL_Renderer *renderer);

//Draw status bar with image info and zoom
void draw_status_bar(SDL_Renderer *renderer, int img_width, int img_height, int zoom_percent);

//Draw complete UI frame (toolbar, ribbon, tabs, status bar)
void draw_ui_frame(SDL_Renderer *renderer, const char *filename, int img_width, int img_height, int zoom_percent);

//Redraw complete UI with icons and borders
void redraw_complete_ui(SDL_Renderer *renderer, SDL_Rect *rects, const char *filename, int img_width, int img_height, int zoom_percent);

//Draw tool labels below icons
void draw_tool_labels(SDL_Renderer *renderer, SDL_Rect *rects);

#endif

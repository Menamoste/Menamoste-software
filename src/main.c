#include <stdio.h>
#include "err_compat.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "tools.h"
#include "color.h"
#include "path.h"
#include "selection.h"
#include "app_init.h"
#include "app_data.h"
#include "event_handler.h"

#define PATH_MAX_LENGTH 35

//Constants
const size_t window0_width = 600;
const size_t window0_height = 200;
const size_t window_width = 1920;
const size_t window_height = 1080;
const size_t icon_size = 96;
const size_t nb_rects = 18;
const size_t nb_colors = 9;
const size_t nb_icons = 8;
const size_t path_max_length = PATH_MAX_LENGTH;
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	// Initialize SDL and create window/renderer
	AppContext* app_ctx = app_init_sdl(window_width, window_height, "Menamoste Image Editor");
	if (!app_ctx) {
		SDL_Log("Erreur : Failed to initialize SDL\n");
		return -1;
	}

	SDL_Renderer *renderer = app_ctx->renderer;
	SDL_Window *window = app_ctx->window;

	// Initialize color palette
	SDL_Color colors[9];
	app_init_colors(colors);

	// Initialize filter matrices
	matrix *convo, *convo2;
	app_init_filter_matrices(&convo, &convo2);

	// Setup rectangles
	SDL_Rect rects[nb_rects];
	place_rects(rects);

	// Load icon variants (normal and select)
	load_icon_variants(renderer);

	// Clear background
	SDL_SetRenderDrawColor(renderer, 60, 62, 68, 255);
	SDL_RenderClear(renderer);

	// Draw toolbar background
	draw_toolbar_bg(renderer);

	// Draw ribbon
	draw_top_ribbon(renderer, "Lenna.bmp");

	// Draw tabs
	draw_image_tabs(renderer);

	// Render the icons
	for (size_t i = 0; i < nb_icons; i++) {
		printf("Rendering icon %zu at (%d, %d)\n", i, rects[i].x, rects[i].y);
		render_icon_variant(renderer, rects[i], i, 0);
		draw_button_border(renderer, rects[i]);
	}

	// Draw tool labels below icons
	draw_tool_labels(renderer, rects);

	// Draw status bar
	draw_status_bar(renderer, 0, 0, 100);

	// Print the result on the renderer
	SDL_RenderPresent(renderer);

	// Load default image (Lenna.bmp)
	SDL_Surface *image_surface = SDL_LoadBMP("res/Images/Lenna.bmp");
	if (!image_surface) {
		printf("Error: Could not load default image res/Images/Lenna.bmp: %s\n", SDL_GetError());
		app_cleanup_sdl(app_ctx);
		app_cleanup_filter_matrices(convo, convo2);
		return -1;
	}

	// Get the matrix
	matrix_pack *mat_pack = sur_to_mat_pack(image_surface);

	// Set the image coordinates (positioned in upper area of canvas)
	SDL_Rect *image_rect = &rects[nb_rects - 1];
	int canvas_height = window_height - 35 - 330;  // Status bar height = 35, toolbar = 330
	int image_y = 330 + (canvas_height / 6);  // Position in upper portion of canvas
	set_rect(image_rect, window_width / 3, image_y, 0, 0);

	// Draw background and UI frame
	SDL_SetRenderDrawColor(renderer, 60, 62, 68, 255);
	SDL_RenderClear(renderer);
	draw_ui_frame(renderer, "Lenna.bmp", image_surface->w, image_surface->h, 100);

	// Re-render icons and borders on top of UI frame
	for (size_t i = 0; i < nb_icons; i++) {
		printf("Initial render icon %zu\n", i);
		render_icon_variant(renderer, rects[i], i, 0);
		draw_button_border(renderer, rects[i]);
	}

	// Draw tool labels
	draw_tool_labels(renderer, rects);

	// Render color swatches
	printf("Rendering color palette\n");
	int active_color_idx = 0;
	draw_color_palette(renderer, rects, colors, active_color_idx);

	// Draw the image on top
	print_image(renderer, image_rect, image_surface, mat_pack);
	draw_canvas_border(renderer, *image_rect);
	SDL_RenderPresent(renderer);

	printf("Image loaded successfully. Click toolbar buttons to use tools.\n");

	// Create event context and run event loop
	EventContext event_ctx = {
		.window = window,
		.renderer = renderer,
		.colors = colors,
		.rects = rects,
		.image_surface = image_surface,
		.mat_pack = mat_pack,
		.convo = convo,
		.convo2 = convo2,
		.active_tool = -1,
		.active_color_idx = 0,
		.nb_icons = nb_icons,
		.nb_rects = nb_rects,
		.window_width = window_width,
		.window_height = window_height
	};

	event_loop_run(&event_ctx);

	// Cleanup
	app_cleanup_filter_matrices(convo, convo2);
	app_cleanup_sdl(app_ctx);
	TTF_Quit();
	return 0;
}

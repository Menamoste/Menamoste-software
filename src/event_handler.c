#include "event_handler.h"
#include "gui.h"
#include "tools.h"
#include "color.h"
#include "path.h"
#include "selection.h"
#include "app_data.h"
#include <stdlib.h>
#include <stdio.h>

int event_loop_run(EventContext* ctx)
{
	if (!ctx) return -1;

	// Event loop state variables
	char opened = 1;
	char is_resized = 0;
	char is_pencil = 0;
	char is_selection = 0;
	size_t is_selected = 0;
	int mouse_x = 0;
	int mouse_y = 0;
	SDL_Event events;

	// Color of the pencil
	triplet trip = {1.0f, 0.0f, 0.0f};

	// Selection coordinates
	size_t x1 = 0;
	size_t y1 = 0;
	size_t x2 = 0;
	size_t y2 = 0;

	while (opened)
	{
		while (SDL_PollEvent(&events))
		{
			switch (events.type)
			{
			case SDL_QUIT:
				opened--;
				break;

			case SDL_MOUSEBUTTONDOWN:
				SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
				SDL_Point mouse_pos = {mouse_x, mouse_y};

				// Pencil tool
				if (SDL_PointInRect(&mouse_pos, &ctx->rects[0]))
				{
					printf("Pencil button clicked (active_tool=%d)\n", ctx->active_tool);
					if (ctx->active_tool != 0) {
						if (ctx->active_tool >= 0) {
							printf("  Deactivating tool %d\n", ctx->active_tool);
							draw_toolbar_bg(ctx->renderer);
							for (size_t i = 0; i < ctx->nb_icons; i++) {
								render_icon_variant(ctx->renderer, ctx->rects[i], i, (i == 0) ? 1 : 0);
								if (i == 0) {
									draw_active_border(ctx->renderer, ctx->rects[i], 1);
								} else {
									draw_button_border(ctx->renderer, ctx->rects[i]);
								}
							}
							// Clear color palette area when switching tools
							SDL_SetRenderDrawColor(ctx->renderer, 100, 100, 100, 255);
							SDL_Rect color_area = {ctx->window_width - 260, 90, 220, 220};
							SDL_RenderFillRect(ctx->renderer, &color_area);
						}
						ctx->active_tool = 0;
						// Show color palette for pencil tool
						draw_color_palette(ctx->renderer, ctx->rects, ctx->colors, ctx->active_color_idx);
						SDL_RenderPresent(ctx->renderer);
					}
					is_pencil = 1;
				}

				// Filter tool
				if (SDL_PointInRect(&mouse_pos, &ctx->rects[4]))
				{
					if (is_resized == 0)
					{
						matrix_pack *mat_pack2 = modify_image(ctx->mat_pack, ctx->convo, 1);
						SDL_SetRenderDrawColor(ctx->renderer, 60, 62, 68, 255);
						SDL_RenderClear(ctx->renderer);
						draw_ui_frame(ctx->renderer, "Lenna.bmp", ctx->image_surface->w, ctx->image_surface->h, 100);
						for (size_t i = 0; i < ctx->nb_icons; i++) {
							render_icon_variant(ctx->renderer, ctx->rects[i], i, 0);
							draw_button_border(ctx->renderer, ctx->rects[i]);
						}
						print_image(ctx->renderer, &ctx->rects[ctx->nb_rects - 1], ctx->image_surface, mat_pack2);
						draw_canvas_border(ctx->renderer, ctx->rects[ctx->nb_rects - 1]);
						SDL_RenderPresent(ctx->renderer);
						is_pencil = 0;
					}
				}

				// Selection tool
				if (SDL_PointInRect(&mouse_pos, &ctx->rects[5]))
				{
					if (ctx->active_tool != 5) {
						if (ctx->active_tool >= 0) {
							render_icon_variant(ctx->renderer, ctx->rects[ctx->active_tool], ctx->active_tool, 0);
							draw_button_border(ctx->renderer, ctx->rects[ctx->active_tool]);
						}
						render_icon_variant(ctx->renderer, ctx->rects[5], 5, 1);
						draw_active_border(ctx->renderer, ctx->rects[5], 1);
						// Clear color palette when switching away from pencil
						SDL_SetRenderDrawColor(ctx->renderer, 100, 100, 100, 255);
						SDL_Rect color_area = {ctx->window_width - 260, 90, 220, 220};
						SDL_RenderFillRect(ctx->renderer, &color_area);
						ctx->active_tool = 5;
						SDL_RenderPresent(ctx->renderer);
					}
					is_selection = 1;
				}

				// Resize tool
				if (SDL_PointInRect(&mouse_pos, &ctx->rects[6]))
				{
					if (is_resized == 0)
					{
						SDL_Rect arg1_rect = {1700, 400, 200, 50};
						SDL_Rect arg2_rect = {1700, 600, 200, 50};
						fill_text_box(ctx->renderer, arg2_rect);

						char *arg1 = text_box(ctx->renderer, arg1_rect, 5);
						char *arg2 = text_box(ctx->renderer, arg2_rect, 5);
						int new_h = atoi(arg1);
						int new_w = atoi(arg2);

						matrix_pack *mat_pack2 = resize_image(ctx->mat_pack, new_w, new_h);

						SDL_Color color[1];
						set_color(color, 0, 100, 100, 100, 0);
						draw_rect(ctx->renderer, ctx->rects[ctx->nb_rects - 1], color[0]);

						ctx->image_surface = SDL_CreateRGBSurface(0,
							mat_pack2->r->cols,
							mat_pack2->r->rows,
							32, 0, 0, 0, 0);
						print_image(ctx->renderer, &ctx->rects[ctx->nb_rects - 1], ctx->image_surface, mat_pack2);
						draw_canvas_border(ctx->renderer, ctx->rects[ctx->nb_rects - 1]);
						draw_status_bar(ctx->renderer, mat_pack2->r->cols, mat_pack2->r->rows, 100);
						SDL_RenderPresent(ctx->renderer);
						is_pencil = 0;
						is_resized++;

						free(arg1);
						free(arg2);
					}
				}

				// Rotate tool
				if (SDL_PointInRect(&mouse_pos, &ctx->rects[7]))
				{
					matrix_pack *mat_pack2 = modify_image(ctx->mat_pack, ctx->convo, 3);
					print_image(ctx->renderer, &ctx->rects[ctx->nb_rects - 1], ctx->image_surface, mat_pack2);
					draw_canvas_border(ctx->renderer, ctx->rects[ctx->nb_rects - 1]);
					SDL_RenderPresent(ctx->renderer);
					is_pencil = 0;
				}

				// Image clicks
				if (SDL_PointInRect(&mouse_pos, &ctx->rects[ctx->nb_rects - 1]))
				{
					if (is_pencil == 1)
					{
						int rel_x = mouse_x - ctx->window_width / 3;
						int rel_y = mouse_y - ctx->window_height / 4;
						color_pixel(ctx->mat_pack, ctx->image_surface, trip, rel_x, rel_y);
						print_image(ctx->renderer, &ctx->rects[ctx->nb_rects - 1], ctx->image_surface, ctx->mat_pack);
						draw_canvas_border(ctx->renderer, ctx->rects[ctx->nb_rects - 1]);
						SDL_RenderPresent(ctx->renderer);
					}

					if (is_selection)
					{
						x1 = 0;
						y1 = 0;
						SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
						while ((!x1 && !y1))
						{
							if (SDL_MOUSEBUTTONDOWN)
							{
								x1 = mouse_x - ctx->window_width / 3;
								y1 = mouse_y - ctx->window_height / 4;
							}
						}
					}
				}

				// Color swatch clicks
				for (size_t i = ctx->nb_icons; i < ctx->nb_rects - 1; i++) {
					if (SDL_PointInRect(&mouse_pos, &ctx->rects[i])) {
						color_to_trip(ctx->colors[i - ctx->nb_icons], &trip);
						ctx->active_color_idx = (int)(i - ctx->nb_icons);
						// Redraw all color swatches to update selection
						for (size_t j = ctx->nb_icons; j < ctx->nb_rects - 1; j++) {
							draw_color_swatch(ctx->renderer, ctx->rects[j], ctx->colors[j - ctx->nb_icons],
											(int)(j - ctx->nb_icons) == ctx->active_color_idx);
						}
						SDL_RenderPresent(ctx->renderer);
						break;
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
				SDL_Point mouse_pos_up = {mouse_x, mouse_y};

				// Image selection finalization
				if (SDL_PointInRect(&mouse_pos_up, &ctx->rects[ctx->nb_rects - 1])) {
					if (is_selection)
					{
						x2 = 0;
						y2 = 0;
						while ((!x2 && !y2))
						{
							if (SDL_MOUSEBUTTONUP)
							{
								x2 = mouse_x - ctx->window_width / 3;
								y2 = mouse_y - ctx->window_height / 4;

								SDL_Color color[1];
								set_color(color, 0, 100, 100, 100, 0);
								draw_rect(ctx->renderer, ctx->rects[ctx->nb_rects - 1], color[0]);

								matrix_pack* mat_pack2 = select(ctx->mat_pack, x1, y1, x2, y2, &is_selected);

								ctx->image_surface = SDL_CreateRGBSurface(0,
									mat_pack2->r->cols,
									mat_pack2->r->rows,
									32, 0, 0, 0, 0);

								print_image(ctx->renderer, &ctx->rects[ctx->nb_rects - 1],
									ctx->image_surface, mat_pack2);
								draw_canvas_border(ctx->renderer, ctx->rects[ctx->nb_rects - 1]);
								SDL_RenderPresent(ctx->renderer);

								is_pencil = 0;
								is_selection = 0;
							}
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

#include "toolbar.h"
#include "ui_config.h"
#include "sdl2_ttf_compat.h"
#include <stdio.h>

extern const size_t icon_size;

void toolbar_render_background(SDL_Renderer* renderer, size_t window_width)
{
    // Fill toolbar background
    SDL_SetRenderDrawColor(renderer, TOOLBAR_BG_R, TOOLBAR_BG_G, TOOLBAR_BG_B, TOOLBAR_BG_A);
    SDL_Rect toolbar_rect = {0, 0, window_width, TOOLBAR_HEIGHT};
    SDL_RenderFillRect(renderer, &toolbar_rect);

    // Draw bottom separator line
    SDL_SetRenderDrawColor(renderer, SEPARATOR_R, SEPARATOR_G, SEPARATOR_B, SEPARATOR_A);
    SDL_RenderDrawLine(renderer, 0, TOOLBAR_HEIGHT - 1, window_width, TOOLBAR_HEIGHT - 1);
    SDL_RenderDrawLine(renderer, 0, TOOLBAR_HEIGHT, window_width, TOOLBAR_HEIGHT);

    // Draw vertical separator between tool groups (Bucket and Filter)
    int sep_x = TOOL_BUTTON_START_X + TOOL_BUTTON_SPACING * 3 + icon_size + (TOOL_BUTTON_SPACING - icon_size) / 2;
    SDL_RenderDrawLine(renderer, sep_x, SEPARATOR_Y_TOP, sep_x, SEPARATOR_Y_BOTTOM);
    SDL_RenderDrawLine(renderer, sep_x + 1, SEPARATOR_Y_TOP, sep_x + 1, SEPARATOR_Y_BOTTOM);
}

void toolbar_render_tool_labels(SDL_Renderer* renderer, SDL_Rect* rects, size_t nb_icons)
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

    TTF_Font *font = TTF_OpenFont("res/Fonts/arial.ttf", TOOL_LABEL_FONT_SIZE);
    if (!font) return;

    SDL_Color text_color = {TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_B, TEXT_COLOR_A};

    for (size_t i = 0; i < nb_icons; i++) {
        SDL_Surface *text_surf = TTF_RenderText_Blended(font, tool_names[i], text_color);
        if (text_surf) {
            SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
            if (text_tex) {
                // Position text below the icon, centered
                int text_x = rects[i].x + (icon_size - text_surf->w) / 2;
                int text_y = rects[i].y + icon_size + TOOL_LABEL_OFFSET_Y;
                SDL_Rect text_rect = {text_x, text_y, text_surf->w, text_surf->h};
                SDL_RenderCopy(renderer, text_tex, NULL, &text_rect);
                SDL_DestroyTexture(text_tex);
            }
            SDL_FreeSurface(text_surf);
        }
    }

    TTF_CloseFont(font);
}

#include "app_data.h"
#include "gui.h"

void app_init_colors(SDL_Color* colors)
{
    // Initialize color palette (9 colors)
    set_color(colors, 0, 255, 0, 0, 255);      // Red
    set_color(colors, 1, 0, 255, 0, 255);      // Green
    set_color(colors, 2, 0, 0, 255, 255);      // Blue
    set_color(colors, 3, 255, 0, 255, 255);    // Magenta
    set_color(colors, 4, 0, 255, 255, 255);    // Cyan
    set_color(colors, 5, 255, 255, 0, 255);    // Yellow
    set_color(colors, 6, 255, 255, 255, 255);  // White
    set_color(colors, 7, 127, 127, 127, 255);  // Gray
    set_color(colors, 8, 0, 0, 0, 255);        // Black
}

void app_init_filter_matrices(matrix** contrast_matrix, matrix** blur_matrix)
{
    // Contrast/sharpening matrix (3x3)
    *contrast_matrix = matrix_zero(3, 3);
    matrix_set(*contrast_matrix, 0, 0, 0);
    matrix_set(*contrast_matrix, 0, 1, -1);
    matrix_set(*contrast_matrix, 0, 2, 0);
    matrix_set(*contrast_matrix, 1, 0, -1);
    matrix_set(*contrast_matrix, 1, 1, 5);
    matrix_set(*contrast_matrix, 1, 2, -1);
    matrix_set(*contrast_matrix, 2, 0, 0);
    matrix_set(*contrast_matrix, 2, 1, -1);
    matrix_set(*contrast_matrix, 2, 2, 0);

    // Box blur matrix (3x3)
    *blur_matrix = matrix_zero(3, 3);
    float blur_val = 0.1111111f;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix_set(*blur_matrix, i, j, blur_val);
        }
    }
}

void app_cleanup_filter_matrices(matrix* contrast_matrix, matrix* blur_matrix)
{
    if (contrast_matrix) matrix_free(contrast_matrix);
    if (blur_matrix) matrix_free(blur_matrix);
}

void color_to_trip(SDL_Color color, triplet *trip)
{
    trip->r = (float)color.r / 255.0f;
    trip->g = (float)color.g / 255.0f;
    trip->b = (float)color.b / 255.0f;
}

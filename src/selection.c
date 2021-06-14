#include <stdio.h>
#include "matrix.h"
#include "selection.h"
#include <err.h>
#include <stdlib.h>

void swap(size_t *a, size_t *b)
{
    size_t temp = *a;
    *a = *b;
    *b = temp;
}

void check_coordinates(size_t* x1, size_t* y1, size_t* x2, size_t* y2)
{
    if (*x1 > *x2)
    {
        swap(x1, x2);
    }
    if (*y1 > *y2)
    {
        swap(y1, y2);
    }
}

void mat_pack_set_selection(matrix_pack* mat_pack, size_t x, size_t y, triplet color, size_t x1, size_t y1)
{
    size_t x_offset = x - x1; 
    size_t y_offset = y - y1;
    mat_pack_set(mat_pack, x_offset, y_offset, color);
}

char is_in_selection(size_t x, size_t y, size_t x1, size_t y1, size_t x2, size_t y2)
{
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

matrix_pack* select(matrix_pack* mat_pack, size_t x1, size_t y1, size_t x2, size_t y2, size_t* is_selected)
{
    matrix_pack* result = mat_pack;
    size_t height = result->r->rows;
    size_t width = result->r->cols;
    *is_selected = 1;

    if (x1 > width)
    {
        err(EXIT_FAILURE,"select: x1 Out of range");
    }
    if (x2 > width)
    {
        err(EXIT_FAILURE,"select: x2 Out of range");
    }
    if (y1 > height)
    {
        err(EXIT_FAILURE,"select: y1 Out of range");
    }
    if (y2 > height)
    {
        err(EXIT_FAILURE,"select: y2 Out of range");
    }

    check_coordinates(&x1, &y1, &x2, &y2);                
    triplet color = {100, 100, 100};


    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            if (!is_in_selection(x, y, x1, y1, x2, y2))
            {
                mat_pack_set(result, y, x, color);
                printf("x: %lu - y: %lu\n", x, y);
            }
        } 
    }

    return result;

}

matrix_pack* crop(matrix_pack* mat_pack, size_t x1, size_t y1, size_t x2, size_t y2)
{
    size_t height = mat_pack->r->rows;
    size_t width = mat_pack->r->cols;

    if (x1 > width)
    {
        err(EXIT_FAILURE,"crop: x1 Out of range");
    }
    if (x2 > width)
    {
        err(EXIT_FAILURE,"crop: x2 Out of range");
    }
    if (y1 > height)
    {
        err(EXIT_FAILURE,"crop: y1 Out of range");
    }
    if (y2 > height)
    {
        err(EXIT_FAILURE,"crop: y2 Out of range");
    }

    check_coordinates(&x1, &y1, &x2, &y2);

    size_t newH = y2 - y1;
    size_t newW = x2 - x1;

    matrix_pack* result = mat_pack_zero(newH, newW);
    
    for (size_t x = x1; x < x2; x++)
    {
        for (size_t y = y1; y < y2; y++)
        {
            triplet color = mat_pack_get(mat_pack, y, x);
        	mat_pack_set_selection(result, y, x, color, y1, x1);
            //printf("x: %lu - y: %lu\n", x, y);
        }
    }

    return result;
}
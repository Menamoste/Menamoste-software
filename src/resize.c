#include <stdio.h>
#include "matrix.h"

matrix_pack* resize(matrix_pack* mat_pack, size_t new_w, size_t new_h)
{
    size_t rows = mat_pack->r->rows;
    size_t cols = mat_pack->r->cols;
    matrix_pack* result = mat_pack_zero(new_h, new_w);
    for (size_t x = 0; x < new_h; x++)
    {
        for (size_t y = 0; y < new_w; y++)
        {
            triplet color = mat_pack_get(mat_pack, (x * cols) / new_w,
	    (y * rows) / new_h);
            mat_pack_set(result, x, y, color); 
        }
    }
    return result;
}

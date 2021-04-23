#include <stdio.h>
#include "matrix.h"

matrix_pack* resize(matrix_pack* mat_pack, size_t new_w, size_t new_h)
{
    size_t rows = mat_pack->r->rows;
    size_t cols = mat_pack->r->cols;
    matrix_pack* result = mat_pack_zero(new_w, new_h);
    for (size_t x = 0; x < new_w; x++)
    {
        for (size_t y = 0; y < new_h; y++)
        {
	    size_t new_x = (x * cols) / new_w;
	    size_t new_y = (y * rows) / new_h;

	    if (new_x < rows && new_y < cols) 
	    {
  	          triplet color = mat_pack_get(mat_pack, new_x, new_y);
        	  mat_pack_set(result, x, y, color); 
	    }
        }
    }
    return result;
}

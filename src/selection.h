#include <stdio.h>
#include "matrix.h"

#ifndef SELECTION_H
#define SELECTION_H

void swap(size_t *a, size_t *b);
void check_coordinates(size_t* x1, size_t* y1, size_t* x2, size_t* y2);
void mat_pack_set_selection(matrix_pack* mat_pack, size_t x, size_t y, triplet color, size_t x1, size_t y1);
char is_in_selection(size_t x, size_t y, size_t x1, size_t y1, size_t x2, size_t y2);
matrix_pack *select(matrix_pack* mat_pack, size_t x1, size_t y1, size_t x2, size_t y2, size_t* is_selected);
matrix_pack *crop(matrix_pack* mat_pack, size_t x1, size_t y1, size_t x2, size_t y2);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "matrix.h"

matrix *matrix_new(size_t rows, size_t cols) {
    matrix *mat = malloc(sizeof(matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(sizeof(unsigned char) * rows * cols);
    return mat;
}

matrix *matrix_zero(size_t rows, size_t cols) {
    matrix *mat = matrix_new(rows, cols);
    memset(mat->data, 0, rows * cols * sizeof(unsigned char));
    return mat;
}

void matrix_free(matrix *mat) {
	free(mat->data);
	free(mat);
}

unsigned char matrix_get(const matrix *mat, size_t i, size_t j) {
	if (i > mat->rows || j > mat->cols)
		err(1, "Coordinates out of bounds.");
	return MAT_GET(mat, i, j);
}

void matrix_set(matrix *mat, size_t i, size_t j, unsigned char val) {
	if (i > mat->rows || j > mat->cols)
		err(1, "Coordinates out of bounds.");
	MAT_GET(mat, i, j) = val;
}

matrix_pack *mat_pack_zero(size_t rows, size_t cols) {
	matrix_pack *mat_pack = malloc(sizeof(matrix_pack));
	mat_pack->r = matrix_zero(rows, cols);
	mat_pack->g = matrix_zero(rows, cols);
	mat_pack->b = matrix_zero(rows, cols);
	return mat_pack;
}

void mat_pack_free(matrix_pack *mat_pack) {
	free(mat_pack->r);
	free(mat_pack->g);
	free(mat_pack->b);
	free(mat_pack);
}

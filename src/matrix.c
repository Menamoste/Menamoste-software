#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "matrix.h"

//Matrices functions:
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

void matrix_add(matrix *mat, unsigned char val) {
	for (size_t i = 0; i < mat->rows; ++i) {
		for (size_t j = 0; j < mat->cols; ++j) {
			MAT_GET(mat, i, j) = val;
		}
	}
}

//Mat_pack functions :
matrix_pack *mat_pack_zero(size_t rows, size_t cols) {
	matrix_pack *mat_pack = malloc(sizeof(matrix_pack));
	mat_pack->r = matrix_zero(rows, cols);
	mat_pack->g = matrix_zero(rows, cols);
	mat_pack->b = matrix_zero(rows, cols);
	return mat_pack;
}

void mat_pack_free(matrix_pack *mat_pack) {
	matrix_free(mat_pack->r);
	matrix_free(mat_pack->g);
	matrix_free(mat_pack->b);
	free(mat_pack);
}

triplet mat_pack_get(matrix_pack *mat_pack, size_t i, size_t j) {
	unsigned char r = matrix_get(mat_pack->r, i, j);
	unsigned char g = matrix_get(mat_pack->g, i, j);
	unsigned char b = matrix_get(mat_pack->b, i, j);
	triplet trip = {r, g, b};
	return trip;
}

void mat_pack_set(matrix_pack *mat_pack, size_t i, size_t j, triplet trip) {
	matrix_set(mat_pack->r, i, j, trip.r);
	matrix_set(mat_pack->g, i, j, trip.g);
	matrix_set(mat_pack->b, i, j, trip.b);
}

void mat_pack_add(matrix_pack *mat_pack, unsigned char val) {
	matrix_add(mat_pack->r, val);
	matrix_add(mat_pack->g, val);
	matrix_add(mat_pack->b, val);
}

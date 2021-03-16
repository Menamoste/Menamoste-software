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
    mat->data = malloc(sizeof(float) * rows * cols);
    return mat;
}

matrix *matrix_zero(size_t rows, size_t cols) {
    matrix *mat = matrix_new(rows, cols);
    memset(mat->data, 0, rows * cols * sizeof(float));
    return mat;
}

void matrix_free(matrix *mat) {
	free(mat->data);
	free(mat);
}

float matrix_get(const matrix *mat, size_t i, size_t j) {
	if (i > mat->rows || j > mat->cols)
		err(EXIT_FAILURE, "Coordinates out of bounds.");
	return MAT_GET(mat, i, j);
}

void matrix_set(matrix *mat, size_t i, size_t j, float val) {
	if (i > mat->rows || j > mat->cols)
		err(EXIT_FAILURE, "Coordinates out of bounds.");
	MAT_GET(mat, i, j) = val;
}

void matrix_add(matrix *mat, float val) {
	for (size_t i = 0; i < mat->rows; ++i) {
		for (size_t j = 0; j < mat->cols; ++j) {
			MAT_GET(mat, i, j) = val;
		}
	}
}

matrix *matrix_multiply(matrix *mat1, matrix *mat2) {
	size_t rows = mat2->rows;
	size_t cols = mat1->cols;

	if (rows != cols)
		err(EXIT_FAILURE, "Could not multiply.");
		
 	matrix *result = matrix_new(rows, cols);

        for (size_t i = 0; i < rows; ++i) {
        	for (size_t j = 0; j < cols; ++j) {
            		float sum = 0.f;
            		for (size_t k = 0; k < cols; ++k)
                		sum += MAT_GET(mat1, i, k) * MAT_GET(mat2, k, j);
            		MAT_GET(result, i, j) = sum;
        	}
	}
    return result;
}

void print_matrix(matrix *mat) {
	size_t rows = mat->rows;
	size_t cols = mat->cols;
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			printf("%f ", matrix_get(mat, i, j));
		}
		printf("\n");
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
	float r = matrix_get(mat_pack->r, i, j);
	float g = matrix_get(mat_pack->g, i, j);
	float b = matrix_get(mat_pack->b, i, j);
	triplet trip = {r, g, b};
	return trip;
}

void mat_pack_set(matrix_pack *mat_pack, size_t i, size_t j, triplet trip) {
	matrix_set(mat_pack->r, i, j, trip.r);
	matrix_set(mat_pack->g, i, j, trip.g);
	matrix_set(mat_pack->b, i, j, trip.b);
}

void mat_pack_add(matrix_pack *mat_pack, float val) {
	matrix_add(mat_pack->r, val);
	matrix_add(mat_pack->g, val);
	matrix_add(mat_pack->b, val);
}

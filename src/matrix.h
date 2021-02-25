#include <stdio.h>

#ifndef MATRIX_H
#define MATRIX_H

//Unsafe get
#define MAT_GET(mat, i, j) (mat->data[i * mat->cols + j])

//Struct for matrices.
typedef struct {
	size_t         rows;
	size_t         cols;
	unsigned char *data;
} matrix;

//Never forget to free !

//Struct for package of three matrices, used for picture RGB.

typedef struct {
	matrix *r;
	matrix *g;
	matrix *b;
} matrix_pack;

//Create a new matrix with random values.
matrix *matrix_new(size_t rows, size_t cols);

//Create a new matrix filled with zeroes.
matrix *matrix_zero(size_t rows, size_t cols);

//Free a matrix.
void matrix_free(matrix *mat);

//Get the value in i and j on the matrix mat.
unsigned char matrix_get(const matrix *mat, size_t i, size_t j);

//Set the value in i and j on the matrix mat.
void matrix_set(matrix *mat, size_t i, size_t j, unsigned char val);

//Create a new matrix_pack filled with zeros.
matrix_pack *mat_pack_zero(size_t rows, size_t cols);

// Free a matrix_pack.
void mat_pack_free(matrix_pack *mat_pack);
#endif

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

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} triplet;

//Create a new matrix with random values. Generally, to forbid.
matrix *matrix_new(size_t rows, size_t cols);

//Create a new matrix filled with zeroes.
matrix *matrix_zero(size_t rows, size_t cols);

//Free a matrix.
void matrix_free(matrix *mat);

//Get the value in i and j on the matrix mat.
unsigned char matrix_get(const matrix *mat, size_t i, size_t j);

//Set the value in i and j on the matrix mat.
void matrix_set(matrix *mat, size_t i, size_t j, unsigned char val);

//Add the value val on the matrix mat.
void matrix_add(matrix *mat, unsigned char val);

//Create a new matrix_pack filled with zeros.
matrix_pack *mat_pack_zero(size_t rows, size_t cols);

//Free a matrix_pack.
void mat_pack_free(matrix_pack *mat_pack);

//Gives a triplet from i and j on the mat_pack.
triplet mat_pack_get(matrix_pack *mat_pack, size_t i, size_t j);

//Set a tripet at i and j on the mat_pack.
void mat_pack_set(matrix_pack *mat_pack, size_t i, size_t j, triplet trip);

//Add the value val on the matrix_pack mat_pack.
void mat_pack_add(matrix_pack *mat_pack, unsigned char val); 
#endif

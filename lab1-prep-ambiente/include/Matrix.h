#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Matrix Matrix;

// Criação e destruição
Matrix* matrix_create(size_t rows, size_t cols);
void matrix_destroy(Matrix* matrix);

// Informações da matriz
size_t matrix_rows(const Matrix* matrix);
size_t matrix_cols(const Matrix* matrix);

// Manipulação de elementos
bool matrix_set(Matrix* matrix, size_t row, size_t col, double value);
double matrix_get(const Matrix* matrix, size_t row, size_t col);

// Preenchimento de matrizes
void matrix_fill(Matrix* matrix, double value);
void matrix_zeros(Matrix* matrix);
void matrix_ones(Matrix* matrix);
void matrix_random(Matrix* matrix, double min, double max);
bool matrix_identity(Matrix* matrix);

// Operações com matrizes
bool matrix_add(const Matrix* a, const Matrix* b, Matrix* result);
bool matrix_subtract(const Matrix* a, const Matrix* b, Matrix* result);
bool matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result);

// Operações com escalares
void matrix_scale(const Matrix* matrix, double scalar, Matrix* result);
void matrix_add_scalar(const Matrix* matrix, double scalar, Matrix* result);
void matrix_subtract_scalar(const Matrix* matrix, double scalar, Matrix* result);

// Operações avançadas
bool matrix_transpose(const Matrix* matrix, Matrix* result);
double matrix_determinant(const Matrix* matrix);
bool matrix_inverse(const Matrix* matrix, Matrix* result);

// Utilitários
void matrix_print(const Matrix* matrix);
bool matrix_save(const Matrix* matrix, const char* filename);
Matrix* matrix_load(const char* filename);

#endif // MATRIX_H
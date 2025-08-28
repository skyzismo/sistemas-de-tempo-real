#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Matrix Matrix;

// Cria uma nova matriz com as dimensões especificadas
Matrix* matrix_create(size_t rows, size_t cols);

// Destroi a matriz e libera a memória
void matrix_destroy(Matrix* matrix);

// Retorna o número de linhas da matriz
size_t matrix_rows(const Matrix* matrix);

// Retorna o número de colunas da matriz
size_t matrix_cols(const Matrix* matrix);

// Define o valor de um elemento na posição (row, col)
bool matrix_set(Matrix* matrix, size_t row, size_t col, double value);

// Obtém o valor de um elemento na posição (row, col)
double matrix_get(const Matrix* matrix, size_t row, size_t col);

// Preenche a matriz com um valor específico
void matrix_fill(Matrix* matrix, double value);

// Preenche a matriz com zeros
void matrix_zeros(Matrix* matrix);

// Preenche a matriz com uns
void matrix_ones(Matrix* matrix);

// Preenche a matriz com valores aleatórios entre min e max
void matrix_random(Matrix* matrix, double min, double max);

// Cria uma matriz identidade (deve ser quadrada)
bool matrix_identity(Matrix* matrix);

// Soma duas matrizes (result = a + b)
bool matrix_add(const Matrix* a, const Matrix* b, Matrix* result);

// Subtrai duas matrizes (result = a - b)
bool matrix_subtract(const Matrix* a, const Matrix* b, Matrix* result);

// Multiplica duas matrizes (result = a * b)
bool matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result);

// Multiplica a matriz por um escalar (result = matrix * scalar)
void matrix_scale(const Matrix* matrix, double scalar, Matrix* result);

// Transpõe a matriz
bool matrix_transpose(const Matrix* matrix, Matrix* result);

// Calcula o determinante da matriz (apenas para matrizes quadradas)
double matrix_determinant(const Matrix* matrix);

// Calcula a matriz inversa (apenas para matrizes quadradas invertíveis)
bool matrix_inverse(const Matrix* matrix, Matrix* result);

// Imprime a matriz na saída padrão
void matrix_print(const Matrix* matrix);

// Salva a matriz em um arquivo
bool matrix_save(const Matrix* matrix, const char* filename);

// Carrega a matriz de um arquivo
Matrix* matrix_load(const char* filename);

#endif // MATRIX_H
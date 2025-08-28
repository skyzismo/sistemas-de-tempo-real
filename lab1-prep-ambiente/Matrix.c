#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MATRIX_EPSILON 1e-10

struct Matrix {
    size_t rows;
    size_t cols;
    double* data;
};

Matrix* matrix_create(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
        return NULL;
    }

    Matrix* matrix = malloc(sizeof(Matrix));
    if (!matrix) {
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * cols * sizeof(double));
    
    if (!matrix->data) {
        free(matrix);
        return NULL;
    }

    return matrix;
}

void matrix_destroy(Matrix* matrix) {
    if (matrix) {
        free(matrix->data);
        free(matrix);
    }
}

size_t matrix_rows(const Matrix* matrix) {
    return matrix ? matrix->rows : 0;
}

size_t matrix_cols(const Matrix* matrix) {
    return matrix ? matrix->cols : 0;
}

bool matrix_set(Matrix* matrix, size_t row, size_t col, double value) {
    if (!matrix || row >= matrix->rows || col >= matrix->cols) {
        return false;
    }
    matrix->data[row * matrix->cols + col] = value;
    return true;
}

double matrix_get(const Matrix* matrix, size_t row, size_t col) {
    if (!matrix || row >= matrix->rows || col >= matrix->cols) {
        return 0.0;
    }
    return matrix->data[row * matrix->cols + col];
}

void matrix_fill(Matrix* matrix, double value) {
    if (!matrix) return;
    for (size_t i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->data[i] = value;
    }
}

void matrix_zeros(Matrix* matrix) {
    matrix_fill(matrix, 0.0);
}

void matrix_ones(Matrix* matrix) {
    matrix_fill(matrix, 1.0);
}

void matrix_random(Matrix* matrix, double min, double max) {
    if (!matrix) return;
    
    srand(time(NULL));
    double range = max - min;
    
    for (size_t i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->data[i] = min + ((double)rand() / RAND_MAX) * range;
    }
}

bool matrix_identity(Matrix* matrix) {
    if (!matrix || matrix->rows != matrix->cols) {
        return false;
    }
    
    matrix_zeros(matrix);
    for (size_t i = 0; i < matrix->rows; i++) {
        matrix->data[i * matrix->cols + i] = 1.0;
    }
    return true;
}

bool matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!a || !b || !result || 
        a->rows != b->rows || a->cols != b->cols ||
        a->rows != result->rows || a->cols != result->cols) {
        return false;
    }
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
    return true;
}

bool matrix_subtract(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!a || !b || !result || 
        a->rows != b->rows || a->cols != b->cols ||
        a->rows != result->rows || a->cols != result->cols) {
        return false;
    }
    
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
    return true;
}

bool matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (!a || !b || !result || 
        a->cols != b->rows || 
        a->rows != result->rows || b->cols != result->cols) {
        return false;
    }
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += matrix_get(a, i, k) * matrix_get(b, k, j);
            }
            matrix_set(result, i, j, sum);
        }
    }
    return true;
}

void matrix_scale(const Matrix* matrix, double scalar, Matrix* result) {
    if (!matrix || !result || 
        matrix->rows != result->rows || matrix->cols != result->cols) {
        return;
    }
    
    for (size_t i = 0; i < matrix->rows * matrix->cols; i++) {
        result->data[i] = matrix->data[i] * scalar;
    }
}

bool matrix_transpose(const Matrix* matrix, Matrix* result) {
    if (!matrix || !result || 
        matrix->rows != result->cols || matrix->cols != result->rows) {
        return false;
    }
    
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            matrix_set(result, j, i, matrix_get(matrix, i, j));
        }
    }
    return true;
}

static double matrix_determinant_recursive(const Matrix* matrix) {
    if (matrix->rows == 1) {
        return matrix_get(matrix, 0, 0);
    }
    if (matrix->rows == 2) {
        return matrix_get(matrix, 0, 0) * matrix_get(matrix, 1, 1) -
               matrix_get(matrix, 0, 1) * matrix_get(matrix, 1, 0);
    }
    
    double det = 0.0;
    int sign = 1;
    
    for (size_t j = 0; j < matrix->cols; j++) {
        // Criar submatriz menor
        Matrix* minor = matrix_create(matrix->rows - 1, matrix->cols - 1);
        if (!minor) continue;
        
        for (size_t row = 1; row < matrix->rows; row++) {
            size_t col_index = 0;
            for (size_t col = 0; col < matrix->cols; col++) {
                if (col != j) {
                    matrix_set(minor, row - 1, col_index++, matrix_get(matrix, row, col));
                }
            }
        }
        
        det += sign * matrix_get(matrix, 0, j) * matrix_determinant_recursive(minor);
        sign *= -1;
        matrix_destroy(minor);
    }
    
    return det;
}

double matrix_determinant(const Matrix* matrix) {
    if (!matrix || matrix->rows != matrix->cols) {
        return 0.0;
    }
    return matrix_determinant_recursive(matrix);
}

bool matrix_inverse(const Matrix* matrix, Matrix* result) {
    if (!matrix || !result || matrix->rows != matrix->cols || 
        matrix->rows != result->rows || matrix->cols != result->cols) {
        return false;
    }
    
    double det = matrix_determinant(matrix);
    if (fabs(det) < MATRIX_EPSILON) {
        return false;
    }
    
    if (matrix->rows == 1) {
        matrix_set(result, 0, 0, 1.0 / matrix_get(matrix, 0, 0));
        return true;
    }
    
    // Para matrizes maiores, usar método de cofatores
    Matrix* cofactor = matrix_create(matrix->rows, matrix->cols);
    if (!cofactor) return false;
    
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            // Criar matriz menor
            Matrix* minor = matrix_create(matrix->rows - 1, matrix->cols - 1);
            if (!minor) {
                matrix_destroy(cofactor);
                return false;
            }
            
            size_t minor_row = 0;
            for (size_t row = 0; row < matrix->rows; row++) {
                if (row == i) continue;
                size_t minor_col = 0;
                for (size_t col = 0; col < matrix->cols; col++) {
                    if (col == j) continue;
                    matrix_set(minor, minor_row, minor_col++, matrix_get(matrix, row, col));
                }
                minor_row++;
            }
            
            double cofactor_value = ((i + j) % 2 == 0 ? 1 : -1) * matrix_determinant(minor);
            matrix_set(cofactor, i, j, cofactor_value);
            matrix_destroy(minor);
        }
    }
    
    Matrix* adjugate = matrix_create(matrix->rows, matrix->cols);
    if (!adjugate) {
        matrix_destroy(cofactor);
        return false;
    }
    
    matrix_transpose(cofactor, adjugate);
    matrix_scale(adjugate, 1.0 / det, result);
    
    matrix_destroy(cofactor);
    matrix_destroy(adjugate);
    return true;
}

void matrix_print(const Matrix* matrix) {
    if (!matrix) {
        printf("Matrix is NULL\n");
        return;
    }
    
    printf("Matrix %zux%zu:\n", matrix->rows, matrix->cols);
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            printf("%8.4f ", matrix_get(matrix, i, j));
        }
        printf("\n");
    }
}

bool matrix_save(const Matrix* matrix, const char* filename) {
    if (!matrix || !filename) return false;
    
    FILE* file = fopen(filename, "w");
    if (!file) return false;
    
    fprintf(file, "%zu %zu\n", matrix->rows, matrix->cols);
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            fprintf(file, "%.15g ", matrix_get(matrix, i, j));
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    return true;
}

Matrix* matrix_load(const char* filename) {
    if (!filename) return NULL;
    
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    size_t rows, cols;
    if (fscanf(file, "%zu %zu", &rows, &cols) != 2) {
        fclose(file);
        return NULL;
    }
    
    Matrix* matrix = matrix_create(rows, cols);
    if (!matrix) {
        fclose(file);
        return NULL;
    }
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            double value;
            if (fscanf(file, "%lf", &value) != 1) {
                matrix_destroy(matrix);
                fclose(file);
                return NULL;
            }
            matrix_set(matrix, i, j, value);
        }
    }
    
    fclose(file);
    return matrix;
}
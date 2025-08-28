#include <stdio.h>
#include <math.h>
#include "Matrix.h"
#include "Integral.h"

// Funções de exemplo para integração
double linear_function(double x) {
    return 2 * x + 1;
}

double quadratic_function(double x) {
    return x * x;
}

double exponential_function(double x) {
    return exp(x);
}

double trigonometric_function(double x) {
    return sin(x);
}

// Função para integração dupla
double function_2d(double x, double y) {
    return x*x + y*y;
}

int main() {
    printf("=== Teste do ADT Matrix ===\n");
    
    // Criar e testar matrizes
    Matrix* A = matrix_create(3, 3);
    matrix_random(A, 1.0, 5.0);
    printf("Matriz A:\n");
    matrix_print(A);
    
    Matrix* B = matrix_create(3, 3);
    matrix_identity(B);
    printf("\nMatriz identidade B:\n");
    matrix_print(B);
    
    Matrix* C = matrix_create(3, 3);
    matrix_add(A, B, C);
    printf("\nA + B:\n");
    matrix_print(C);
    
    printf("\nDeterminante de A: %.4f\n", matrix_determinant(A));
    
    matrix_destroy(A);
    matrix_destroy(B);
    matrix_destroy(C);
    
    printf("\n=== Teste do ADT Integral ===\n");
    
    // Testar diferentes métodos de integração
    IntegralConfig config = integral_config_create(quadratic_function, 0.0, 1.0);
    
    printf("∫x² dx de 0 a 1 (valor esperado: ~0.3333)\n");
    
    integral_config_set_method(&config, INTEGRAL_METHOD_RECTANGLE_LEFT);
    IntegralResult result = integral_calculate(&config);
    printf("Retângulo esquerdo: %.6f (erro: %.2e)\n", result.result, result.error_estimate);
    
    integral_config_set_method(&config, INTEGRAL_METHOD_RECTANGLE_MIDPOINT);
    result = integral_calculate(&config);
    printf("Ponto médio: %.6f (erro: %.2e)\n", result.result, result.error_estimate);
    
    integral_config_set_method(&config, INTEGRAL_METHOD_TRAPEZOID);
    result = integral_calculate(&config);
    printf("Trapézio: %.6f (erro: %.2e)\n", result.result, result.error_estimate);
    
    integral_config_set_method(&config, INTEGRAL_METHOD_SIMPSON);
    result = integral_calculate(&config);
    printf("Simpson: %.6f (erro: %.2e)\n", result.result, result.error_estimate);
    
    // Testar integral dupla - CORRIGIDO
    printf("\nIntegral dupla ∫∫(x² + y²) dxdy de 0 a 1:\n");
    double double_result = integral_double(
        function_2d,  // Passando a função diretamente
        0.0, 1.0, 0.0, 1.0, 100, INTEGRAL_METHOD_SIMPSON
    );
    printf("Resultado: %.6f (esperado: ~0.6667)\n", double_result);
    
    return 0;
}
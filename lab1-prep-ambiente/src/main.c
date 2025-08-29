#include <stdio.h>
#define _USE_MATH_DEFINES  // Deve ser definido ANTES de incluir math.h
#include <math.h>
#include <time.h>
#include "../include/Integral.h"
#include "../include/Matrix.h"

// ==================== FUNÇÕES PARA TESTE DE INTEGRAL ====================

double linear_function(double x) {
    return 2 * x + 1;
}

double quadratic_function(double x) {
    return x * x;
}

double cubic_function(double x) {
    return x * x * x;
}

double exponential_function(double x) {
    return exp(x);
}

double trigonometric_function(double x) {
    return sin(x);
}

double complex_function(double x) {
    return sin(x) * exp(-x);
}

// ==================== TESTES DA MATRIX ====================

void test_matrix_operations() {
    printf("\n=== TESTE DE OPERAÇÕES MATRICIAIS ===\n");
    
    // Teste 1: Criação e operações básicas
    printf("\n1. Criação e operações básicas:\n");
    
    Matrix* A = matrix_create(2, 2);
    matrix_set(A, 0, 0, 1.0);
    matrix_set(A, 0, 1, 2.0);
    matrix_set(A, 1, 0, 3.0);
    matrix_set(A, 1, 1, 4.0);
    printf("Matriz A:\n");
    matrix_print(A);
    
    Matrix* B = matrix_create(2, 2);
    matrix_set(B, 0, 0, 5.0);
    matrix_set(B, 0, 1, 6.0);
    matrix_set(B, 1, 0, 7.0);
    matrix_set(B, 1, 1, 8.0);
    printf("Matriz B:\n");
    matrix_print(B);
    
    Matrix* C = matrix_create(2, 2);
    matrix_add(A, B, C);
    printf("A + B:\n");
    matrix_print(C);
    
    matrix_subtract(B, A, C);
    printf("B - A:\n");
    matrix_print(C);
    
    // Teste 2: Multiplicação
    printf("\n2. Multiplicação matricial:\n");
    Matrix* D = matrix_create(2, 3);
    matrix_random(D, 1.0, 5.0);
    printf("Matriz D (2x3):\n");
    matrix_print(D);
    
    Matrix* E = matrix_create(3, 2);
    matrix_random(E, 1.0, 5.0);
    printf("Matriz E (3x2):\n");
    matrix_print(E);
    
    Matrix* F = matrix_create(2, 2);
    matrix_multiply(D, E, F);
    printf("D * E:\n");
    matrix_print(F);
    
    // Teste 3: Operações com escalar
    printf("\n3. Operações com escalar:\n");
    matrix_scale(A, 2.0, C);
    printf("2 * A:\n");
    matrix_print(C);
    
    matrix_add_scalar(A, 10.0, C);
    printf("A + 10:\n");
    matrix_print(C);
    
    // Teste 4: Matriz identidade e transposta
    printf("\n4. Matriz identidade e transposta:\n");
    Matrix* I = matrix_create(3, 3);
    matrix_identity(I);
    printf("Matriz identidade 3x3:\n");
    matrix_print(I);
    
    Matrix* G = matrix_create(2, 3);
    matrix_random(G, 1.0, 9.0);
    printf("Matriz G (2x3):\n");
    matrix_print(G);
    
    Matrix* Gt = matrix_create(3, 2);
    matrix_transpose(G, Gt);
    printf("Transposta de G:\n");
    matrix_print(Gt);
    
    // Teste 5: Determinante e inversa
    printf("\n5. Determinante e inversa:\n");
    Matrix* H = matrix_create(3, 3);
    matrix_set(H, 0, 0, 4.0); matrix_set(H, 0, 1, 7.0); matrix_set(H, 0, 2, 2.0);
    matrix_set(H, 1, 0, 3.0); matrix_set(H, 1, 1, 5.0); matrix_set(H, 1, 2, 1.0);
    matrix_set(H, 2, 0, 8.0); matrix_set(H, 2, 1, 2.0); matrix_set(H, 2, 2, 6.0);
    printf("Matriz H:\n");
    matrix_print(H);
    
    double det = matrix_determinant(H);
    printf("Determinante de H: %.4f\n", det);
    
    Matrix* H_inv = matrix_create(3, 3);
    if (matrix_inverse(H, H_inv)) {
        printf("Inversa de H:\n");
        matrix_print(H_inv);
        
        // Verificação: H * H_inv deve ser a identidade
        Matrix* verify = matrix_create(3, 3);
        matrix_multiply(H, H_inv, verify);
        printf("H * H_inv (deve ser identidade):\n");
        matrix_print(verify);
        matrix_destroy(verify);
    } else {
        printf("Matriz H não é invertível\n");
    }
    
    // Limpeza
    matrix_destroy(A); matrix_destroy(B); matrix_destroy(C);
    matrix_destroy(D); matrix_destroy(E); matrix_destroy(F);
    matrix_destroy(G); matrix_destroy(Gt); matrix_destroy(H);
    matrix_destroy(H_inv); matrix_destroy(I);
}

// ==================== TESTES DE INTEGRAL ====================

void test_integral_methods() {
    printf("\n=== TESTE DE MÉTODOS DE INTEGRAÇÃO ===\n");
    
    // Definir manualmente as constantes se não estiverem definidas
    #ifndef M_PI
    #define M_PI 3.14159265358979323846
    #endif
    
    #ifndef M_E
    #define M_E 2.71828182845904523536
    #endif
    
    // Teste com diferentes funções
    struct TestFunction {
        MathFunction func;
        const char* name;
        double a;
        double b;
        double expected;
    };
    
    struct TestFunction tests[] = {
        {linear_function, "2x + 1", 0.0, 1.0, 2.0},
        {quadratic_function, "x²", 0.0, 1.0, 1.0/3.0},
        {cubic_function, "x³", 0.0, 1.0, 0.25},
        {trigonometric_function, "sin(x)", 0.0, M_PI, 2.0},
        {exponential_function, "exp(x)", 0.0, 1.0, M_E - 1.0}
    };
    
    IntegralMethod methods[] = {
        INTEGRAL_METHOD_RECTANGLE_LEFT,
        INTEGRAL_METHOD_RECTANGLE_RIGHT,
        INTEGRAL_METHOD_RECTANGLE_MIDPOINT,
        INTEGRAL_METHOD_TRAPEZOID,
        INTEGRAL_METHOD_SIMPSON,
        INTEGRAL_METHOD_TRAPEZOID_COMPOSITE,
        INTEGRAL_METHOD_SIMPSON_COMPOSITE
    };
    
    const char* method_names[] = {
        "Retângulo Esquerdo",
        "Retângulo Direito",
        "Ponto Médio",
        "Trapézio",
        "Simpson",
        "Trapézio Composto",
        "Simpson Composto"
    };
    
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        printf("\n∫ %s dx de %.1f a %.1f (esperado: %.6f)\n", 
               tests[i].name, tests[i].a, tests[i].b, tests[i].expected);
        printf("==============================================\n");
        
        for (size_t j = 0; j < sizeof(methods)/sizeof(methods[0]); j++) {
            IntegralConfig config = integral_config_create(tests[i].func, tests[i].a, tests[i].b);
            integral_config_set_method(&config, methods[j]);
            integral_config_set_intervals(&config, 1000);
            
            IntegralResult result = integral_calculate(&config);
            double error = fabs(result.result - tests[i].expected);
            
            printf("%-18s: %10.6f (erro: %8.2e, eval: %4zu)\n",
                   method_names[j], result.result, error, result.function_evaluations);
        }
    }
    
    // Teste com Romberg
    printf("\n\n=== MÉTODO DE ROMBERG ===\n");
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        double result = integral_romberg(tests[i].func, tests[i].a, tests[i].b, 1e-8, 10);
        double error = fabs(result - tests[i].expected);
        printf("Romberg ∫ %s: %.6f (erro: %.2e)\n", tests[i].name, result, error);
    }
}

// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    printf("=== TESTE COMPLETO DOS ADTs ===\n");
    
    // Testar Matrix
    test_matrix_operations();
    
    // Testar Integral
    test_integral_methods();
    
    printf("\n=== TESTES CONCLUÍDOS ===\n");
    return 0;
}
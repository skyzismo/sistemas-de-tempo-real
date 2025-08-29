#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <stddef.h>

// Tipo de função para integração
typedef double (*MathFunction)(double);

// Métodos de integração disponíveis
typedef enum {
    INTEGRAL_METHOD_RECTANGLE_LEFT,     // Somas de Riemann - esquerda
    INTEGRAL_METHOD_RECTANGLE_RIGHT,    // Somas de Riemann - direita
    INTEGRAL_METHOD_RECTANGLE_MIDPOINT, // Regra do Ponto Médio
    INTEGRAL_METHOD_TRAPEZOID,          // Regra do Trapézio
    INTEGRAL_METHOD_SIMPSON,            // Regra de Simpson
    INTEGRAL_METHOD_TRAPEZOID_COMPOSITE,// Regra Composta do Trapézio
    INTEGRAL_METHOD_SIMPSON_COMPOSITE   // Regra Composta de Simpson
} IntegralMethod;

// Estrutura para configuração de integração
typedef struct {
    MathFunction function;
    double a;
    double b;
    size_t n;           // Número de subintervalos
    IntegralMethod method;
    double tolerance;
    size_t max_iterations;
} IntegralConfig;

// Resultado da integração
typedef struct {
    double result;
    double error_estimate;
    size_t iterations;
    size_t function_evaluations;
} IntegralResult;

// Cria uma nova configuração de integração
IntegralConfig integral_config_create(MathFunction func, double a, double b);

// Define o método de integração
void integral_config_set_method(IntegralConfig* config, IntegralMethod method);

// Define o número de subintervalos
void integral_config_set_intervals(IntegralConfig* config, size_t n);

// Define a tolerância
void integral_config_set_tolerance(IntegralConfig* config, double tolerance);

// Define o número máximo de iterações
void integral_config_set_max_iterations(IntegralConfig* config, size_t max_iter);

// Calcula a integral usando a configuração especificada
IntegralResult integral_calculate(const IntegralConfig* config);

// Métodos de integração individuais
double integral_rectangle_left(MathFunction f, double a, double b, size_t n);
double integral_rectangle_right(MathFunction f, double a, double b, size_t n);
double integral_rectangle_midpoint(MathFunction f, double a, double b, size_t n);
double integral_trapezoid(MathFunction f, double a, double b, size_t n);
double integral_simpson(MathFunction f, double a, double b, size_t n);
double integral_trapezoid_composite(MathFunction f, double a, double b, size_t n);
double integral_simpson_composite(MathFunction f, double a, double b, size_t n);

// Método de Romberg
double integral_romberg(MathFunction f, double a, double b, double tolerance, size_t max_iter);

#endif // INTEGRAL_H
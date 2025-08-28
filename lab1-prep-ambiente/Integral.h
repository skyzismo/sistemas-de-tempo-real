#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <stddef.h>

// Tipo de função para integração
typedef double (*MathFunction)(double);

// Tipo de função para integração dupla
typedef double (*MathFunction2D)(double, double);

// Métodos de integração disponíveis
typedef enum {
    INTEGRAL_METHOD_RECTANGLE_LEFT,
    INTEGRAL_METHOD_RECTANGLE_RIGHT,
    INTEGRAL_METHOD_RECTANGLE_MIDPOINT,
    INTEGRAL_METHOD_TRAPEZOID,
    INTEGRAL_METHOD_SIMPSON
} IntegralMethod;

// Estrutura para configuração de integração
typedef struct {
    MathFunction function;
    double a;
    double b;
    size_t n;           // Número de subintervalos
    IntegralMethod method;
    double tolerance;   // Tolerância para métodos adaptativos
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

// Define a tolerância para métodos adaptativos
void integral_config_set_tolerance(IntegralConfig* config, double tolerance);

// Define o número máximo de iterações
void integral_config_set_max_iterations(IntegralConfig* config, size_t max_iter);

// Calcula a integral usando a configuração especificada
IntegralResult integral_calculate(const IntegralConfig* config);

// Métodos de integração diretos
double integral_rectangle_left(MathFunction f, double a, double b, size_t n);
double integral_rectangle_right(MathFunction f, double a, double b, size_t n);
double integral_rectangle_midpoint(MathFunction f, double a, double b, size_t n);
double integral_trapezoid(MathFunction f, double a, double b, size_t n);
double integral_simpson(MathFunction f, double a, double b, size_t n);

// Método adaptativo (recursivo)
double integral_adaptive(MathFunction f, double a, double b, double tolerance, int max_depth);

// Integração multidimensional (dupla)
double integral_double(MathFunction2D f, double a, double b, double c, double d, 
                      size_t n, IntegralMethod method);

#endif // INTEGRAL_H
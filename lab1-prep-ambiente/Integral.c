#include "Integral.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

IntegralConfig integral_config_create(MathFunction func, double a, double b) {
    IntegralConfig config = {
        .function = func,
        .a = a,
        .b = b,
        .n = 1000,
        .method = INTEGRAL_METHOD_SIMPSON,
        .tolerance = 1e-8,
        .max_iterations = 1000
    };
    return config;
}

void integral_config_set_method(IntegralConfig* config, IntegralMethod method) {
    if (config) config->method = method;
}

void integral_config_set_intervals(IntegralConfig* config, size_t n) {
    if (config && n > 0) config->n = n;
}

void integral_config_set_tolerance(IntegralConfig* config, double tolerance) {
    if (config && tolerance > 0) config->tolerance = tolerance;
}

void integral_config_set_max_iterations(IntegralConfig* config, size_t max_iter) {
    if (config && max_iter > 0) config->max_iterations = max_iter;
}

IntegralResult integral_calculate(const IntegralConfig* config) {
    IntegralResult result = {0};
    
    if (!config || !config->function || config->a >= config->b) {
        return result;
    }
    
    switch (config->method) {
        case INTEGRAL_METHOD_RECTANGLE_LEFT:
            result.result = integral_rectangle_left(config->function, config->a, config->b, config->n);
            result.function_evaluations = config->n;
            break;
            
        case INTEGRAL_METHOD_RECTANGLE_RIGHT:
            result.result = integral_rectangle_right(config->function, config->a, config->b, config->n);
            result.function_evaluations = config->n;
            break;
            
        case INTEGRAL_METHOD_RECTANGLE_MIDPOINT:
            result.result = integral_rectangle_midpoint(config->function, config->a, config->b, config->n);
            result.function_evaluations = config->n;
            break;
            
        case INTEGRAL_METHOD_TRAPEZOID:
            result.result = integral_trapezoid(config->function, config->a, config->b, config->n);
            result.function_evaluations = config->n + 1;
            break;
            
        case INTEGRAL_METHOD_SIMPSON:
            result.result = integral_simpson(config->function, config->a, config->b, config->n);
            result.function_evaluations = 2 * config->n + 1;
            break;
    }
    
    // Estimativa simples de erro (pode ser melhorada)
    if (config->n >= 2) {
        double result_h = result.result;
        double result_h2 = integral_calculate(&(IntegralConfig){
            .function = config->function,
            .a = config->a,
            .b = config->b,
            .n = config->n / 2,
            .method = config->method
        }).result;
        
        result.error_estimate = fabs(result_h - result_h2);
    }
    
    return result;
}

double integral_rectangle_left(MathFunction f, double a, double b, size_t n) {
    if (n == 0) return 0.0;
    
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (size_t i = 0; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    
    return sum * h;
}

double integral_rectangle_right(MathFunction f, double a, double b, size_t n) {
    if (n == 0) return 0.0;
    
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (size_t i = 1; i <= n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    
    return sum * h;
}

double integral_rectangle_midpoint(MathFunction f, double a, double b, size_t n) {
    if (n == 0) return 0.0;
    
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (size_t i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    
    return sum * h;
}

double integral_trapezoid(MathFunction f, double a, double b, size_t n) {
    if (n == 0) return 0.0;
    
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));
    
    for (size_t i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    
    return sum * h;
}

double integral_simpson(MathFunction f, double a, double b, size_t n) {
    if (n == 0 || n % 2 != 0) return 0.0;
    
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    
    for (size_t i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0) {
            sum += 2.0 * f(x);
        } else {
            sum += 4.0 * f(x);
        }
    }
    
    return sum * h / 3.0;
}

static double adaptive_recursive(MathFunction f, double a, double b, 
                               double tolerance, int max_depth, int depth) {
    if (depth >= max_depth) {
        return integral_simpson(f, a, b, 2);
    }
    
    double mid = (a + b) / 2.0;
    double whole = integral_simpson(f, a, b, 2);
    double left = integral_simpson(f, a, mid, 2);
    double right = integral_simpson(f, mid, b, 2);
    
    if (fabs(whole - (left + right)) < 15.0 * tolerance) {
        return left + right + (left + right - whole) / 15.0;
    }
    
    return adaptive_recursive(f, a, mid, tolerance/2, max_depth, depth + 1) +
           adaptive_recursive(f, mid, b, tolerance/2, max_depth, depth + 1);
}

double integral_adaptive(MathFunction f, double a, double b, double tolerance, int max_depth) {
    return adaptive_recursive(f, a, b, tolerance, max_depth, 0);
}

double integral_double(MathFunction2D f, double a, double b, double c, double d, 
                      size_t n, IntegralMethod method) {

    // Primeiro integramos em y para cada x, depois integramos em x
    double h_x = (b - a) / n;
    double h_y = (d - c) / n;
    double result = 0.0;
    
    for (size_t i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h_x;  // Ponto médio em x
        
        // Integrar f(x, y) em y para este x fixo
        double sum_y = 0.0;
        for (size_t j = 0; j < n; j++) {
            double y = c + (j + 0.5) * h_y;  // Ponto médio em y
            sum_y += f(x, y);
        }
        double integral_y = sum_y * h_y;
        
        result += integral_y;
    }
    
    return result * h_x;
    
    // Função auxiliar para integrar em y para um x fixo
    double integrate_y(double x); {
        IntegralConfig config = integral_config_create(f, c, d);
        integral_config_set_method(&config, method);
        integral_config_set_intervals(&config, n);
        return integral_calculate(&config).result;
    }
    
    // Integrar a função auxiliar em x
    IntegralConfig config = integral_config_create(integrate_y, a, b);
    integral_config_set_method(&config, method);
    integral_config_set_intervals(&config, n);
    
    return integral_calculate(&config).result;
}
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
        .method = INTEGRAL_METHOD_SIMPSON_COMPOSITE,
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
            
        case INTEGRAL_METHOD_TRAPEZOID_COMPOSITE:
            result.result = integral_trapezoid_composite(config->function, config->a, config->b, config->n);
            result.function_evaluations = config->n + 1;
            break;
            
        case INTEGRAL_METHOD_SIMPSON_COMPOSITE:
            result.result = integral_simpson_composite(config->function, config->a, config->b, config->n);
            result.function_evaluations = 2 * config->n + 1;
            break;
    }
    
    // Estimativa de erro
    if (config->n >= 4) {
        double result_h = result.result;
        IntegralConfig half_config = *config;
        half_config.n = config->n / 2;
        double result_h2 = integral_calculate(&half_config).result;
        
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

double integral_trapezoid_composite(MathFunction f, double a, double b, size_t n) {
    // Implementação composta com refinamento iterativo
    if (n < 2) n = 2;
    
    double previous_result = 0.0;
    double current_result = integral_trapezoid(f, a, b, n);
    
    for (size_t iter = 0; iter < 10; iter++) {
        previous_result = current_result;
        n *= 2;
        current_result = integral_trapezoid(f, a, b, n);
        
        if (fabs(current_result - previous_result) < 1e-8) {
            break;
        }
    }
    
    return current_result;
}

double integral_simpson_composite(MathFunction f, double a, double b, size_t n) {
    // Implementação composta com refinamento iterativo
    if (n < 4 || n % 2 != 0) n = 4;
    
    double previous_result = 0.0;
    double current_result = integral_simpson(f, a, b, n);
    
    for (size_t iter = 0; iter < 10; iter++) {
        previous_result = current_result;
        n *= 2;
        current_result = integral_simpson(f, a, b, n);
        
        if (fabs(current_result - previous_result) < 1e-8) {
            break;
        }
    }
    
    return current_result;
}

double integral_romberg(MathFunction f, double a, double b, double tolerance, size_t max_iter) {
    if (max_iter < 1) max_iter = 1;
    
    double R[max_iter][max_iter];
    
    // Primeira aproximação (Regra do Trapézio com n=1)
    R[0][0] = (b - a) * (f(a) + f(b)) / 2.0;
    
    for (size_t i = 1; i < max_iter; i++) {
        // Refinamento da regra do trapézio
        size_t n = 1 << i; // 2^i
        double h = (b - a) / n;
        double sum = 0.0;
        
        for (size_t k = 1; k <= n-1; k += 2) {
            sum += f(a + k * h);
        }
        
        R[i][0] = 0.5 * R[i-1][0] + h * sum;
        
        // Extrapolação de Richardson
        for (size_t j = 1; j <= i; j++) {
            R[i][j] = R[i][j-1] + (R[i][j-1] - R[i-1][j-1]) / (pow(4, j) - 1);
        }
        
        // Verificar convergência
        if (i > 0 && fabs(R[i][i] - R[i-1][i-1]) < tolerance) {
            return R[i][i];
        }
    }
    
    return R[max_iter-1][max_iter-1];
}
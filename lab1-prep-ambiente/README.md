# ADTs de Manipulação Matricial em Ponto Flutuante e Cálculo Integral

Biblioteca em C que implementa estruturas de dados abstratas (ADTs) para operações matriciais e métodos de integração numérica.

Desenvolvido por **Lucas Vinícius Gonçalves Gadelha - 22050517**

## Estrutura do Projeto

.
├── Matrix.h          # Interface da ADT Matrix
├── Matrix.c          # Implementação da ADT Matrix
├── Integral.h        # Interface da ADT Integral
├── Integral.c        # Implementação da ADT Integral
├── main.c           # Programa de teste
└── Makefile         # Sistema de build

## Funcionalidades da Matrix

Criação e Destruição

```c
Matrix* matrix_create(size_t rows, size_t cols);
void matrix_destroy(Matrix* matrix);
```

## Operações Básicas

- Acesso a elementos: matrix_set(), matrix_get()
- Preenchimento: matrix_zeros(), matrix_ones(), matrix_random()
- Matriz identidade: matrix_identity()

## Operações Aritméticas

- Soma e subtração de matrizes
- Multiplicação matricial
- Multiplicação por escalar
- Transposta
- Determinante
- Matriz inversa

## Utilitários

- Impressão formatada
- Salvar/Carregar em arquivo

## Funcionalidades de Integração

### Métodos Implementados

```c
typedef enum {
    INTEGRAL_METHOD_RECTANGLE_LEFT,     // Somas de Riemann - esquerda
    INTEGRAL_METHOD_RECTANGLE_RIGHT,    // Somas de Riemann - direita
    INTEGRAL_METHOD_RECTANGLE_MIDPOINT, // Regra do Ponto Médio
    INTEGRAL_METHOD_TRAPEZOID,          // Regra do Trapézio
    INTEGRAL_METHOD_SIMPSON,            // Regra de Simpson
    INTEGRAL_METHOD_TRAPEZOID_COMPOSITE,// Regra Composta do Trapézio
    INTEGRAL_METHOD_SIMPSON_COMPOSITE   // Regra Composta de Simpson
} IntegralMethod;
```

### Configuração Flexível

```c
typedef struct {
    MathFunction function;    // Função a integrar
    double a, b;              // Limites de integração
    size_t n;                 // Número de subintervalos
    IntegralMethod method;    // Método numérico
    double tolerance;         // Tolerância para convergência
    size_t max_iterations;    // Máximo de iterações
} IntegralConfig;
```

### Métodos de Romberg
Integração adaptativa com extrapolação de Richardson para alta precisão

## Compilação e Uso

### Compilação

```bash
make        # Compilação normal
make debug  # Compilação com debug symbols
make clean  # Limpa arquivos compilados
```

### Execução

```bash
make run    # Compila e executa
```

## Testes Implementados

### Testes de Matrizes

- Operações básicas (soma, subtração, multiplicação)
- Matriz identidade e transposta
- Cálculo de determinante e matriz inversa
- Verificação de propriedades matemáticas

### Testes de Integração

- Funções lineares, quadráticas e cúbicas
- Funções trigonométricas e exponenciais
- Comparação entre diferentes métodos numéricos
- Análise de erro e convergência
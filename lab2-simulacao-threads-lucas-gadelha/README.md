# Laboratório 2 — Simulação de Robô Móvel em Tempo Real (Threads)

Desenvolvido por **Lucas Vinícius Gonçalves Gadelha - 22050517**

Esta pasta contém uma **estrutura reutilizável** para os próximos laboratórios. O sistema é dividido em duas tarefas usando **pthread**:

- **Tarefa de Simulação (`plant`)** — período nominal `Ts = 50 ms`: integra o modelo do robô móvel e publica a saída **y_f(t)** (ponto à frente do robô).
- **Tarefa de Geração/Amostragem (`io`)** — período nominal `Ts = 50 ms`: gera **u(t)**, lê **y_f(t)** e armazena no arquivo ASCII os valores de **t, u(t), y_f(t)**.

A troca de dados entre as tarefas é **exclusivamente** via `u` e `y_f`, conforme exigido. O programa executa em `t ∈ [0, 20] s` e cria arquivos de saída em `./out`.

Também são registrados os instantes reais de disparo das tarefas para cálculo de **período T(k)** e **jitter J(k) = T(k) - T_nominal**. Execute sem carga e com carga artificial (CPU busy loop) para comparar as estatísticas.

## Como compilar e executar

Requisitos: `gcc`, `make`, `python3` (com `matplotlib` e `pandas`).

```bash
make            # compila
make run        # executa duas vezes: sem carga e com carga
make plots      # gera gráficos e tabela de estatísticas (./out)
```

Execução manual (exemplos):

```bash
./bin/sim --duration 20 --Ts 0.05 --out ./out --load-ms 0
./bin/sim --duration 20 --Ts 0.05 --out ./out --load-ms 2
```

Arquivos gerados em `./out`:
- `samples_no_load.csv`, `samples_load_2ms.csv` — linhas com `t, v, w, yx, yy, theta`.
- `timing_io_no_load.csv`, `timing_io_load_2ms.csv` — instantes de disparo da tarefa `io`.
- `timing_plant_no_load.csv`, `timing_plant_load_2ms.csv` — instantes de disparo da tarefa `plant`.
- Figuras `traj_*.png`, `components_*.png` e tabela `timing_stats.csv` geradas pelo script Python.

## Hierarquia de diretórios

- `include/` — cabeçalhos reutilizáveis para os próximos labs.
- `src/` — código-fonte em C.
- `scripts/` — script Python para gráficos e estatísticas.
- `out/` — resultados e figuras.
- `Makefile` — automação de build/run/plots.
- `README.md` — este arquivo.

## Modelo e sinais

Modelo no espaço de estados:
\dot{x} = [[sin θ, 0], [cos θ, 0], [0, 1]]·u, com x = [x_c, y_c, θ]^T e u=[v, ω]^T.
Saída frontal: y_f = [x_c + 0.5·D·cos θ, y_c + 0.5·D·sin θ, θ]^T, com D=0.30 m.

Entrada u(t):
- t<0: [0, 0]^T
- 0 ≤ t < 10 s: [1, 0.2π]^T
- t ≥ 10 s: [1, -0.2π]^T

Integração por Euler explícito com período nominal `Ts`.


## Como abrir a GUI

Após gerar os arquivos em `./out` (por exemplo com `make run`), execute:

```bash
make gui
# ou:
python3 scripts/rt_gui.py --out ./out --Ts 0.05
```

A GUI oferece:
- Seleção do conjunto (`no_load` e quaisquer `load_Xms` encontrados).
- Abas para **Trajetória**, **Componentes no tempo** e **Estatísticas T(k)/J(k)**.
- Recarregamento rápido dos arquivos sem precisar fechar o app.


## Executar simulação pela GUI

Na janela **Executar simulação**, você pode:
- Ajustar **Duração**, **Ts** e **Carga (ms/ciclo)**.
- Clicar em **Compilar (make)** para construir o binário se necessário.
- Clicar em **Executar** (usa a carga definida) ou **Executar sem carga**.
Ao finalizar, a GUI atualiza automaticamente os conjuntos no seletor e replota os gráficos.


### Reconstrução completa
```bash
make rebuild   # equivale a 'make clean' seguido de 'make'
```

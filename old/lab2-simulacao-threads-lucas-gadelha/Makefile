# Makefile para Laboratório 2 — Simulação RT com Threads (C) + Gráficos (Python)

CC       := gcc
CFLAGS   := -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE -O2 -Wall -Wextra -std=c11 -pthread
INCLUDES := -Iinclude
SRC      := $(wildcard src/*.c)
OBJ      := $(patsubst src/%.c,build/%.o,$(SRC))
BIN_DIR  := bin
OUT_DIR  := out
BIN      := $(BIN_DIR)/sim

PYTHON   ?= python3

.PHONY: all clean run rebuild plots gui dirs

all: dirs $(BIN)

dirs:
	@mkdir -p build $(BIN_DIR) $(OUT_DIR)

$(BIN): $(OBJ) | dirs
	$(CC) $(CFLAGS) $(OBJ) -o $@ -lm

build/%.o: src/%.c | dirs
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(BIN)
	@echo "==> Executando sem carga"
	$(BIN) --duration 20 --Ts 0.05 --out $(OUT_DIR) --load-ms 0
	@echo "==> Executando com carga (2 ms por ciclo)"
	$(BIN) --duration 20 --Ts 0.05 --out $(OUT_DIR) --load-ms 2

plots:
	$(PYTHON) scripts/plot_results.py --out $(OUT_DIR) --Ts 0.05

gui:
	$(PYTHON) scripts/rt_gui.py --out $(OUT_DIR) --Ts 0.05

clean:
	@rm -rf build $(BIN_DIR) $(OUT_DIR)/*.csv $(OUT_DIR)/*.png

rebuild: clean all
	@echo "Recompilação completa concluída."

import argparse
import tkinter as tk
from tkinter import ttk, messagebox
from tkinter.scrolledtext import ScrolledText
from pathlib import Path
import subprocess
import threading

import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

import pandas as pd
import numpy as np

APP_TITLE = "RT Robot — Visualizador e Executor"

def discover_tags(out_dir: Path):
    tags = []
    no_load = out_dir / "samples_no_load.csv"
    if no_load.exists():
        tags.append("no_load")
    for p in sorted(out_dir.glob("samples_load_*ms.csv")):
        tags.append(p.stem.replace("samples_", ""))
    return tags

def compute_stats(times, Ts):
    times = np.asarray(times)
    if len(times) < 2:
        return None, None
    T = np.diff(times)
    J = T - Ts
    def stats(x):
        return {
            "mean": float(np.mean(x)),
            "var":  float(np.var(x, ddof=0)),
            "std":  float(np.std(x, ddof=0)),
            "min":  float(np.min(x)),
            "max":  float(np.max(x)),
        }
    return stats(T), stats(J)

class App(tk.Tk):
    def __init__(self, out_dir: Path, Ts: float):
        super().__init__()
        self.title(APP_TITLE)
        self.geometry("1200x800")
        self.minsize(1024, 700)

        self.project_root = Path(__file__).resolve().parent.parent
        self.bin_path = self.project_root / "bin" / "sim"

        self.out_dir = out_dir
        self.Ts = Ts
        self.tags = discover_tags(out_dir)
        self._running = False

        self.create_widgets()
        self.refresh_tags()
        if self.tags:
            self.tag_var.set(self.tags[0])
            self.load_and_plot()

    # ---------- UI ----------
    def create_widgets(self):
        # Top controls
        top = ttk.Frame(self, padding=8)
        top.pack(fill="x")

        ttk.Label(top, text="Saídas:").pack(side="left")
        ttk.Label(top, text=str(self.out_dir), foreground="#444").pack(side="left", padx=(6,16))

        ttk.Label(top, text="Conjunto:").pack(side="left")
        self.tag_var = tk.StringVar(value="")
        self.tag_cb = ttk.Combobox(top, textvariable=self.tag_var, state="readonly", width=22)
        self.tag_cb.pack(side="left", padx=6)

        ttk.Button(top, text="Atualizar lista", command=self.refresh_tags).pack(side="left", padx=4)
        ttk.Button(top, text="Replotar", command=self.load_and_plot).pack(side="left", padx=4)

        # Exec frame
        exec_fr = ttk.LabelFrame(self, text="Executar simulação", padding=8)
        exec_fr.pack(fill="x", padx=8, pady=(4,0))

        self.var_duration = tk.DoubleVar(value=20.0)
        self.var_Ts = tk.DoubleVar(value=self.Ts)
        self.var_load = tk.IntVar(value=0)

        ttk.Label(exec_fr, text="Duração [s]").grid(row=0, column=0, sticky="e", padx=4, pady=2)
        ttk.Entry(exec_fr, textvariable=self.var_duration, width=8).grid(row=0, column=1, sticky="w", padx=4)

        ttk.Label(exec_fr, text="Ts [s]").grid(row=0, column=2, sticky="e", padx=4, pady=2)
        ttk.Entry(exec_fr, textvariable=self.var_Ts, width=8).grid(row=0, column=3, sticky="w", padx=4)

        ttk.Label(exec_fr, text="Carga (ms/ciclo)").grid(row=0, column=4, sticky="e", padx=4, pady=2)
        ttk.Entry(exec_fr, textvariable=self.var_load, width=8).grid(row=0, column=5, sticky="w", padx=4)

        ttk.Button(exec_fr, text="Compilar (make)", command=self.build_project).grid(row=0, column=6, padx=8)
        ttk.Button(exec_fr, text="Executar", command=self.start_run).grid(row=0, column=7, padx=4)
        ttk.Button(exec_fr, text="Executar sem carga", command=self.start_run_no_load).grid(row=0, column=8, padx=4)

        for i in range(9):
            exec_fr.grid_columnconfigure(i, weight=0)
        exec_fr.grid_columnconfigure(9, weight=1)

        # Notebook
        nb = ttk.Notebook(self)
        nb.pack(fill="both", expand=True, padx=8, pady=8)

        # Tab Trajectory
        self.tab_traj = ttk.Frame(nb)
        nb.add(self.tab_traj, text="Trajetória y(t)")
        self.fig_traj = plt.Figure(figsize=(5,4), dpi=100)
        self.ax_traj = self.fig_traj.add_subplot(111)
        self.canvas_traj = FigureCanvasTkAgg(self.fig_traj, master=self.tab_traj)
        self.canvas_traj.get_tk_widget().pack(fill="both", expand=True)

        # Tab Components
        self.tab_comp = ttk.Frame(nb)
        nb.add(self.tab_comp, text="Componentes no tempo")
        self.fig_comp = plt.Figure(figsize=(5,4), dpi=100)
        self.ax_comp = self.fig_comp.add_subplot(111)
        self.canvas_comp = FigureCanvasTkAgg(self.fig_comp, master=self.tab_comp)
        self.canvas_comp.get_tk_widget().pack(fill="both", expand=True)

        # Tab Stats
        self.tab_stats = ttk.Frame(nb)
        nb.add(self.tab_stats, text="T(k) e J(k) — Estatísticas")
        cols = ("task","mode","metric","mean","var","std","min","max")
        self.tree = ttk.Treeview(self.tab_stats, columns=cols, show="headings", height=12)
        for c in cols:
            self.tree.heading(c, text=c)
            self.tree.column(c, anchor="center", width=110)
        self.tree.pack(fill="both", expand=True, padx=6, pady=(6,0))

        # Log area
        log_frame = ttk.LabelFrame(self, text="Log", padding=4)
        log_frame.pack(fill="both", expand=True, padx=8, pady=(0,8))
        self.log = ScrolledText(log_frame, height=8)
        self.log.pack(fill="both", expand=True)

        # Status
        self.status = tk.StringVar(value="Pronto.")
        sb = ttk.Label(self, textvariable=self.status, anchor="w", relief="sunken")
        sb.pack(fill="x", side="bottom")

    # ---------- Helpers ----------
    def append_log(self, text):
        self.log.insert("end", text + "\n")
        self.log.see("end")

    def set_running(self, running: bool):
        self._running = running
        state = "disabled" if running else "normal"
        for w in (self.tag_cb,):
            w.configure(state="disabled" if running else "readonly")
        # Disable/enable all Buttons and Entries in exec frame and top area
        for child in self.winfo_children():
            if isinstance(child, ttk.LabelFrame) or isinstance(child, ttk.Frame):
                for w in child.winfo_children():
                    if isinstance(w, ttk.Button) or isinstance(w, ttk.Entry) or isinstance(w, ttk.Combobox):
                        try:
                            if isinstance(w, ttk.Combobox):
                                w.configure(state="disabled" if running else "readonly")
                            else:
                                w.configure(state=state)
                        except tk.TclError:
                            pass
        self.status.set("Executando..." if running else "Pronto.")

    def refresh_tags(self):
        self.tags = discover_tags(self.out_dir)
        self.tag_cb["values"] = self.tags
        if not self.tags:
            self.status.set(f"Nenhum CSV encontrado em {self.out_dir}. Rode a simulação para gerar saídas.")
        else:
            self.status.set(f"Conjuntos disponíveis: {', '.join(self.tags)}")

    # ---------- Build/Run ----------
    def build_project(self):
        def worker():
            self.set_running(True)
            try:
                self.append_log("==> Compilando com 'make'...")
                proc = subprocess.run(["make"], cwd=str(self.project_root), capture_output=True, text=True)
                self.append_log(proc.stdout.strip())
                if proc.returncode != 0:
                    self.append_log(proc.stderr.strip())
                    messagebox.showerror("Erro ao compilar", "Falha ao rodar 'make'. Veja o log.")
                else:
                    self.append_log("✔ Compilado com sucesso.")
            except Exception as e:
                messagebox.showerror("Erro", str(e))
            finally:
                self.set_running(False)
        threading.Thread(target=worker, daemon=True).start()

    def start_run_no_load(self):
        self.var_load.set(0)
        self.start_run()

    def start_run(self):
        try:
            duration = float(self.var_duration.get())
            Ts = float(self.var_Ts.get())
            load = int(self.var_load.get())
        except Exception:
            messagebox.showerror("Parâmetros inválidos", "Verifique os valores de Duração, Ts e Carga.")
            return

        def worker():
            self.set_running(True)
            try:
                if not self.bin_path.exists():
                    self.append_log("Binário não encontrado. Tentando compilar...")
                    proc_m = subprocess.run(["make"], cwd=str(self.project_root), capture_output=True, text=True)
                    self.append_log(proc_m.stdout.strip())
                    if proc_m.returncode != 0:
                        self.append_log(proc_m.stderr.strip())
                        messagebox.showerror("Erro ao compilar", "Não foi possível compilar o projeto.")
                        return

                cmd = [str(self.bin_path), "--duration", str(duration), "--Ts", str(Ts),
                       "--out", str(self.out_dir), "--load-ms", str(load)]
                self.append_log("==> Executando: " + " ".join(cmd))
                proc = subprocess.run(cmd, cwd=str(self.project_root), capture_output=True, text=True)
                self.append_log(proc.stdout.strip())
                if proc.returncode != 0:
                    self.append_log(proc.stderr.strip())
                    messagebox.showerror("Erro na execução", "O processo retornou código diferente de zero. Veja o log.")
                    return
                self.append_log("✔ Execução concluída.")
            except Exception as e:
                messagebox.showerror("Erro", str(e))
            finally:
                # Atualiza lista e gráficos
                self.refresh_tags()
                chosen = "no_load" if load == 0 else f"load_{load}ms"
                if chosen in self.tags:
                    self.tag_var.set(chosen)
                self.load_and_plot()
                self.set_running(False)

        threading.Thread(target=worker, daemon=True).start()

    # ---------- Plots & Stats ----------
    def load_and_plot(self):
        tag = self.tag_var.get()
        if not tag:
            return
        csv = self.out_dir / ("samples_no_load.csv" if tag == "no_load" else f"samples_{tag}.csv")
        if not csv.exists():
            messagebox.showerror("Arquivo não encontrado", f"Não encontrei {csv.name}.")
            return
        try:
            df = pd.read_csv(csv)
        except Exception as e:
            messagebox.showerror("Erro ao ler CSV", str(e))
            return

        # Trajectory
        self.ax_traj.clear()
        self.ax_traj.plot(df["yx"], df["yy"])
        self.ax_traj.set_aspect("equal", adjustable="datalim")
        self.ax_traj.set_title(f"Trajetória y(t) — {tag}")
        self.ax_traj.set_xlabel("y_x [m]")
        self.ax_traj.set_ylabel("y_y [m]")
        self.ax_traj.grid(True)
        self.canvas_traj.draw()

        # Components
        self.ax_comp.clear()
        self.ax_comp.plot(df["t"], df["yx"], label="y_x")
        self.ax_comp.plot(df["t"], df["yy"], label="y_y")
        self.ax_comp.plot(df["t"], df["theta"], label="theta")
        self.ax_comp.set_title(f"y(t) amostrado — {tag}")
        self.ax_comp.set_xlabel("t [s]")
        self.ax_comp.set_ylabel("valor")
        self.ax_comp.grid(True)
        self.ax_comp.legend()
        self.canvas_comp.draw()

        # Stats
        for i in self.tree.get_children():
            self.tree.delete(i)

        def load_times(path):
            if not path.exists():
                return None
            try:
                s = pd.read_csv(path)["t_release"].to_numpy()
                return s
            except Exception:
                return None

        for task in ["io","plant"]:
            timing = self.out_dir / (f"timing_{task}_no_load.csv" if tag == "no_load" else f"timing_{task}_{tag}.csv")
            times = load_times(timing)
            if times is None or len(times) < 2:
                continue
            S_T, S_J = compute_stats(times, self.var_Ts.get())
            if S_T is None:
                continue
            self.tree.insert("", "end", values=(task, tag, "T(k)", f"{S_T['mean']:.6f}", f"{S_T['var']:.6e}", f"{S_T['std']:.6f}", f"{S_T['min']:.6f}", f"{S_T['max']:.6f}"))
            self.tree.insert("", "end", values=(task, tag, "J(k)", f"{S_J['mean']:.6f}", f"{S_J['var']:.6e}", f"{S_J['std']:.6f}", f"{S_J['min']:.6f}", f"{S_J['max']:.6f}"))

        self.status.set(f"Visualizando: {tag}")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default="./out", help="Diretório com CSVs")
    ap.add_argument("--Ts", type=float, default=0.05, help="Período nominal [s]")
    args = ap.parse_args()
    out_dir = Path(args.out)

    app = App(out_dir=out_dir, Ts=args.Ts)
    app.mainloop()

if __name__ == "__main__":
    main()

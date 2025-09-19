import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

def compute_stats(times, Ts):
    times = np.asarray(times)
    T = np.diff(times)  # período real
    J = T - Ts          # jitter
    def stats(x):
        return {
            "mean": np.mean(x),
            "var":  np.var(x, ddof=0),
            "std":  np.std(x, ddof=0),
            "min":  np.min(x),
            "max":  np.max(x),
        }
    return stats(T), stats(J)

def save_stats_table(rows, out_csv):
    df = pd.DataFrame(rows, columns=["task","mode","metric","mean","var","std","min","max"])
    df.to_csv(out_csv, index=False)
    try:
        from caas_jupyter_tools import display_dataframe_to_user
        display_dataframe_to_user("Timing Stats", df)
    except Exception:
        pass

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    ap.add_argument("--Ts", type=float, required=True)
    args = ap.parse_args()
    out = Path(args.out)
    Ts = args.Ts

    # Trajetória e componentes (sem carga e com carga se existir)
    for tag in ["no_load"] + [p.name.split("samples_")[1].split(".")[0] for p in out.glob("samples_load_*ms.csv")]:
        csv = out / (f"samples_{tag}.csv" if tag=="no_load" else f"samples_{tag}.csv")
        if not csv.exists(): 
            continue
        df = pd.read_csv(csv)
        # Figura 1: trajetória yx vs yy
        plt.figure()
        plt.plot(df["yx"], df["yy"])
        plt.axis("equal")
        plt.title(f"Trajetória y(t) — {tag}")
        plt.xlabel("y_x [m]")
        plt.ylabel("y_y [m]")
        plt.grid(True)
        plt.savefig(out / f"traj_{tag}.png", dpi=160)
        plt.close()

        # Figura 2: componentes no tempo
        plt.figure()
        plt.plot(df["t"], df["yx"], label="y_x")
        plt.plot(df["t"], df["yy"], label="y_y")
        plt.plot(df["t"], df["theta"], label="theta")
        plt.legend()
        plt.title(f"y(t) amostrado — {tag}")
        plt.xlabel("t [s]")
        plt.ylabel("valor")
        plt.grid(True)
        plt.savefig(out / f"components_{tag}.png", dpi=160)
        plt.close()

    # Estatísticas de T(k) e J(k)
    rows = []
    def load_times(path):
        if not path.exists(): return None
        s = pd.read_csv(path)["t_release"].to_numpy()
        return s

    for task in ["io","plant"]:
        for mode_tag in ["no_load"] + [p.name.split(f"timing_{task}_")[1].split(".")[0] for p in out.glob(f"timing_{task}_load_*ms.csv")]:
            path = out / (f"timing_{task}_{mode_tag}.csv")
            if not path.exists(): 
                continue
            times = load_times(path)
            if times is None or len(times) < 2: 
                continue
            S_T, S_J = compute_stats(times, Ts)
            rows.append([task, mode_tag, "T(k)", S_T["mean"], S_T["var"], S_T["std"], S_T["min"], S_T["max"]])
            rows.append([task, mode_tag, "J(k)", S_J["mean"], S_J["var"], S_J["std"], S_J["min"], S_J["max"]])

    save_stats_table(rows, out / "timing_stats.csv")

if __name__ == "__main__":
    main()

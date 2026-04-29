import pandas as pd
import matplotlib.pyplot as plt

sizes = [16, 32, 64]
dfs = {}
for L in sizes:
    dfs[L] = pd.concat([
        pd.read_csv(f"sim_data/L{L}_low.csv"),
        pd.read_csv(f"sim_data/L{L}_crit.csv"),
        pd.read_csv(f"sim_data/L{L}_high.csv"),
    ]).sort_values("Temp").reset_index(drop=True)

Tc = 2.2692
observables = [
    ("Mean Absolute Magnetization", r"$\langle|m|\rangle$"),
    ("Susceptibility",              r"$\chi$"),
    ("Mean Energy / Spin",          r"$\langle e \rangle$"),
    ("Specific Heat",               r"$C$"),
]

fig, axes = plt.subplots(2, 2, figsize=(12, 8))
fig.suptitle("2D Ising Model Monte Carlo", fontsize=14)
markers = {16: 'o', 32: 's', 64: '^'}

for ax, (col, ylabel) in zip(axes.flat, observables):
    for L in sizes:
        ax.plot(dfs[L]["Temp"], dfs[L][col],
                marker=markers[L], markersize=3,
                label=f"L={L}")
    ax.axvline(Tc, color='red', linestyle='--', linewidth=1,
               label=r"$T_c$ (Onsager)")
    ax.set_xlabel("T")
    ax.set_ylabel(ylabel)
    ax.legend()
    ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("ising_results.png", dpi=150)
plt.show()
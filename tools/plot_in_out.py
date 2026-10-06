#!/usr/bin/python3
# -*- coding: UTF-8 -*-

__all__ = ['a']
__version__ = '0.1'
__author__ = 'Luis Morales' 

import os
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import gaussian_kde

# Function/class definition
class TimeSeriesApp:
    def __init__(self, directory):
        self.directory = directory
        self.meteoin = os.path.join(self.directory,'meteo.in')
        self.resultsout = os.path.join(self.directory,'results.out')
        self.datamet = None
        self.datares = None

    def read_meteoin(self):
        """Load CSV file into a pandas DataFrame."""
        self.datamet= pd.read_csv(self.meteoin, parse_dates=["datetime"])
        print(f"Loaded {self.meteoin} with {len(self.datamet)} rows.")

    def read_resultsout(self):
        """Load CSV file into a pandas DataFrame."""
        self.datares = pd.read_csv(self.resultsout, parse_dates=["datetime"])
        print(f"Loaded {self.resultsout} with {len(self.datares)} rows.")

    def plot_meteoin(self):
        """ Plot all variables in 'meteo.in' file """
        if (self.datamet is None):
            raise ValueError("No data loaded. Call read_csv() first.")

        # Extract time column
        time = self.datamet.iloc[:, 0]

        # Create subplots: one for each data column (excluding the date column)
        fig, axes = plt.subplots(nrows=len(self.datamet.columns) - 1, ncols=1, sharex=True, figsize=(8, 6))

        # Colors to use (will cycle if more columns than colors)
        colors = ["tab:blue", "tab:orange", "tab:green", "tab:red", "tab:purple"]

        for i, col in enumerate(self.datamet.columns[1:]):
            axes[i].plot(time, self.datamet[col], color=colors[i % len(colors)], label=col, linewidth=0.8)
            axes[i].set_ylabel(col)
            #  axes[i].legend(loc="upper right")
        
        axes[-1].set_xlabel("Date (day)")
        #  plt.suptitle("Time Series Subplots (Shared X-Axis)", fontsize=14)
        plt.tight_layout()
        plt.show()

    def plot_disch(self):
        """Plot the time series."""
        if (self.datamet is None) or  (self.datares is None):
            raise ValueError("No data loaded. Call read_csv() first.")

        print(self.datamet.head())
        print(self.datares.head())
        plt.figure(figsize=(10, 5))
        #  plt.plot(self.datamet["datetime"], self.datamet["runoff_mm"], marker=".", linestyle="-",color="red", label="Obs.")
        plt.plot(self.datamet["datetime"], self.datamet["runoff_mm"], color="red", label="Observed", linewidth=0.5)
        #  plt.plot(self.datares["datetime"], self.datares["q_mm"],marker=".", linestyle="-", color="blue", label="Sim.")
        plt.plot(self.datares["datetime"], self.datares["q_mm"], color="blue", label="Simulated", linewidth=0.5)
        plt.title("Time Series Plot")
        plt.xlabel("Day")
        plt.ylabel("Runoff (mm)")
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        plt.show()

    def plot_disch_regimes(self):
        """Plot the daily and monthly regimes, and smoother histograms"""
            
        if (self.datamet is None) or  (self.datares is None):
            raise ValueError("No data loaded. Call read_csv() first.")

        # ========================================================
        # SETTINGS
        # ========================================================
    
        NAME1 = "Observed"
        NAME2 = "Simulated"
    
        UNIT = r"Runoff (mm)"
    
        COLOR1 = "red"   
        COLOR2 = "blue" 
    
        # ========================================================
        # 1. CREATE Q1 AND Q2
        # ========================================================
    
        q1 = self.datamet.set_index("datetime")["runoff_mm"]
    
        q2 = self.datares.set_index("datetime")["q_mm"]
    
        # Make sure values are numeric
        q1 = pd.to_numeric(
            q1,
            errors="coerce"
        ).dropna()
    
        q2 = pd.to_numeric(
            q2,
            errors="coerce"
        ).dropna()
    
    
        # Sort by date
        q1 = q1.sort_index()
        q2 = q2.sort_index()
    
    
        # ========================================================
        # DIAGNOSTICS
        # ========================================================
    
        print("\n==============================")
        print("Q1 diagnostics")
        print("==============================")
    
        print(q1.head())
        print(q1.describe())
        print("Number of values:", len(q1))
        print("Number of unique values:", q1.nunique())
        print("Minimum:", q1.min())
        print("Maximum:", q1.max())
    
    
        print("\n==============================")
        print("Q2 diagnostics")
        print("==============================")
    
        print(q2.head())
        print(q2.describe())
        print("Number of values:", len(q2))
        print("Number of unique values:", q2.nunique())
        print("Minimum:", q2.min())
        print("Maximum:", q2.max())
    
    
        # ========================================================
        # 2. REMOVE FEBRUARY 29
        # ========================================================
    
        q1_daily = q1[
            ~(
                (q1.index.month == 2) &
                (q1.index.day == 29)
            )
        ]
    
        q2_daily = q2[
            ~(
                (q2.index.month == 2) &
                (q2.index.day == 29)
            )
        ]
    
    
        # ========================================================
        # 3. MULTIYEAR DAILY REGIME
        # ========================================================
    
        q1_daily_stats = q1_daily.groupby(
            q1_daily.index.dayofyear
        ).agg(
            mean="mean",
            q25=lambda x: x.quantile(0.25),
            q75=lambda x: x.quantile(0.75)
        )
    
    
        q2_daily_stats = q2_daily.groupby(
            q2_daily.index.dayofyear
        ).agg(
            mean="mean",
            q25=lambda x: x.quantile(0.25),
            q75=lambda x: x.quantile(0.75)
        )
    
    
        # ========================================================
        # 4. MONTHLY DATA
        # ========================================================
    
        q1_monthly = [
            q1[q1.index.month == month].values
            for month in range(1, 13)
        ]
    
        q2_monthly = [
            q2[q2.index.month == month].values
            for month in range(1, 13)
        ]
    
        months = np.arange(1, 13)
    
        month_labels = [
            "Jan", "Feb", "Mar", "Apr",
            "May", "Jun", "Jul", "Aug",
            "Sep", "Oct", "Nov", "Dec"
        ]
    
    
        # ========================================================
        # 5. CREATE FIGURE
        # ========================================================
    
        fig = plt.figure(
            figsize=(12, 9)
        )
    
        gs = fig.add_gridspec(
            2,
            2,
            height_ratios=[1.15, 1],
            hspace=0.30,
            wspace=0.25
        )
    
    
        # ========================================================
        # PANEL A — MULTIYEAR DAILY REGIME
        # ========================================================
    
        ax_daily = fig.add_subplot(
            gs[0, :]
        )
    
    
        # Q1 mean
        ax_daily.plot(
            q1_daily_stats.index,
            q1_daily_stats["mean"],
            color=COLOR1,
            linewidth=2,
            label=NAME1
        )
    
        # Q1 IQR
        ax_daily.fill_between(
            q1_daily_stats.index,
            q1_daily_stats["q25"],
            q1_daily_stats["q75"],
            color=COLOR1,
            alpha=0.15
        )
    
    
        # Q2 mean
        ax_daily.plot(
            q2_daily_stats.index,
            q2_daily_stats["mean"],
            color=COLOR2,
            linewidth=2,
            label=NAME2
        )
    
        # Q2 IQR
        ax_daily.fill_between(
            q2_daily_stats.index,
            q2_daily_stats["q25"],
            q2_daily_stats["q75"],
            color=COLOR2,
            alpha=0.15
        )
    
    
        # Month boundaries
        month_starts = [
            1, 32, 60, 91,
            121, 152, 182, 213,
            244, 274, 305, 335
        ]
    
        for day in month_starts:
            ax_daily.axvline(
                day,
                linewidth=0.6,
                alpha=0.25
            )
    
    
        ax_daily.set_xlim(
            1,
            365
        )
    
        ax_daily.set_xticks(
            month_starts
        )
    
        ax_daily.set_xticklabels(
            month_labels
        )
    
        ax_daily.set_xlabel(
            "Month"
        )
    
        ax_daily.set_ylabel(
            UNIT
        )
    
        ax_daily.set_title(
            "Multiyear Daily Runoff Regime",
            loc="left",
            fontweight="bold"
        )
    
        ax_daily.legend(
            frameon=False,
            ncol=2
        )
    
        ax_daily.grid(
            axis="y",
            alpha=0.25
        )
    
    
        # ========================================================
        # PANEL B — MONTHLY BOXPLOTS
        # ========================================================
    
        ax_month = fig.add_subplot(
            gs[1, 0]
        )
    
    
        positions1 = months - 0.18
        positions2 = months + 0.18
    
    
        bp1 = ax_month.boxplot(
            q1_monthly,
            positions=positions1,
            widths=0.32,
            patch_artist=True,
            showfliers=False
        )
    
    
        bp2 = ax_month.boxplot(
            q2_monthly,
            positions=positions2,
            widths=0.32,
            patch_artist=True,
            showfliers=False
        )
    
    
        # Q1 — BLUE
        for box in bp1["boxes"]:
            box.set_facecolor(COLOR1)
            box.set_edgecolor(COLOR1)
    
        for item in bp1["whiskers"]:
            item.set_color(COLOR1)
    
        for item in bp1["caps"]:
            item.set_color(COLOR1)
    
        for item in bp1["medians"]:
            item.set_color("black")
            item.set_linewidth(1.2)
    
    
        # Q2 — ORANGE
        for box in bp2["boxes"]:
            box.set_facecolor(COLOR2)
            box.set_edgecolor(COLOR2)
    
        for item in bp2["whiskers"]:
            item.set_color(COLOR2)
    
        for item in bp2["caps"]:
            item.set_color(COLOR2)
    
        for item in bp2["medians"]:
            item.set_color("black")
            item.set_linewidth(1.2)
    
    
        ax_month.set_xticks(
            months
        )
    
        ax_month.set_xticklabels(
            month_labels
        )
    
        ax_month.set_ylabel(
            UNIT
        )
    
        ax_month.set_title(
            "Monthly Runoff Regime",
            loc="left",
            fontweight="bold"
        )
    
        ax_month.grid(
            axis="y",
            alpha=0.25
        )
    
    
        # Legend
        ax_month.plot(
            [],
            [],
            color=COLOR1,
            linewidth=8,
            label=NAME1
        )
    
        ax_month.plot(
            [],
            [],
            color=COLOR2,
            linewidth=8,
            label=NAME2
        )
    
        ax_month.legend(
            frameon=False
        )
    
    
        # ========================================================
        # PANEL C — PROBABILITY DENSITY
        # ========================================================
    
        ax_pdf = fig.add_subplot(
            gs[1, 1]
        )
    
    
        # Keep zero runoff values
        q1_pdf = q1[
            np.isfinite(q1) &
            (q1 >= 0)
        ]
    
        q2_pdf = q2[
            np.isfinite(q2) &
            (q2 >= 0)
        ]
    
    
        print("\n==============================")
        print("PDF diagnostics")
        print("==============================")
    
        print(
            NAME1,
            "n =",
            len(q1_pdf),
            "unique =",
            q1_pdf.nunique()
        )
    
        print(
            NAME2,
            "n =",
            len(q2_pdf),
            "unique =",
            q2_pdf.nunique()
        )
    
    
        # --------------------------------------------------------
        # Q1 KDE
        # --------------------------------------------------------
    
        if (
            len(q1_pdf) >= 2
            and q1_pdf.nunique() > 1
        ):
    
            kde1 = gaussian_kde(
                q1_pdf.values
            )
    
            x1 = np.linspace(
                q1_pdf.min(),
                q1_pdf.max(),
                500
            )
    
            pdf1 = kde1(x1)
    
            ax_pdf.plot(
                x1,
                pdf1,
                color=COLOR1,
                linewidth=2,
                label=NAME1
            )
    
            ax_pdf.fill_between(
                x1,
                pdf1,
                color=COLOR1,
                alpha=0.15
            )
    
        else:
    
            print(
                f"{NAME1}: KDE skipped because "
                "the series has no variability."
            )
    
    
        # --------------------------------------------------------
        # Q2 KDE
        # --------------------------------------------------------
    
        if (
            len(q2_pdf) >= 2
            and q2_pdf.nunique() > 1
        ):
    
            kde2 = gaussian_kde(
                q2_pdf.values
            )
    
            x2 = np.linspace(
                q2_pdf.min(),
                q2_pdf.max(),
                500
            )
    
            pdf2 = kde2(x2)
    
            ax_pdf.plot(
                x2,
                pdf2,
                color=COLOR2,
                linewidth=2,
                label=NAME2
            )
    
            ax_pdf.fill_between(
                x2,
                pdf2,
                color=COLOR2,
                alpha=0.15
            )
    
        else:
    
            print(
                f"{NAME2}: KDE skipped because "
                "the series has no variability."
            )
    
    
        # --------------------------------------------------------
        # PDF formatting
        # --------------------------------------------------------
    
        ax_pdf.set_xlabel(
            UNIT
        )
    
        ax_pdf.set_ylabel(
            "Probability density"
        )
    
        ax_pdf.set_title(
            "Runoff Probability Density",
            loc="left",
            fontweight="bold"
        )
    
        ax_pdf.grid(
            axis="both",
            alpha=0.25
        )
    
        ax_pdf.legend(
            frameon=False
        )
    
    
        # ========================================================
        # 6. GENERAL FORMATTING
        # ========================================================
    
        fig.suptitle(
            "Comparison of Runoff Regimes",
            fontsize=15,
            fontweight="bold",
            y=0.98
        )
    
        plt.tight_layout(
            rect=[0, 0, 1, 0.96]
        )
    
    
        # ========================================================
        # 7. SAVE AND SHOW
        # ========================================================
    
       #   plt.savefig(
            #  "runoff_regimes.png",
            #  dpi=300,
            #  bbox_inches="tight"
        #  )
    
        plt.show()
        
    def run(self):
        """Main execution flow."""
        self.read_meteoin()
        self.read_resultsout()
        self.plot_meteoin()
        self.plot_disch()
        self.plot_disch_regimes()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <directory_path>")
        sys.exit(1)

    directory = sys.argv[1]
    app = TimeSeriesApp(directory)
    app.run()


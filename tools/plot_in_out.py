#!/usr/bin/python3
# -*- coding: UTF-8 -*-

__all__ = ['a']
__version__ = '0.1'
__author__ = 'Luis Morales' 

import os
import sys
import pandas as pd
import matplotlib.pyplot as plt

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
        plt.plot(self.datamet["datetime"], self.datamet["runoff_mm"], marker=".", linestyle="-",color="red", label="Obs.")
        plt.plot(self.datares["datetime"], self.datares["q_mm"],marker=".", linestyle="-", color="blue", label="Sim.")
        plt.title("Time Series Plot")
        plt.xlabel("Day")
        plt.ylabel("Q (m^3/s)")
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        plt.show()

    def run(self):
        """Main execution flow."""
        self.read_meteoin()
        self.read_resultsout()
        self.plot_disch()
        self.plot_meteoin()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <directory_path>")
        sys.exit(1)

    directory = sys.argv[1]
    app = TimeSeriesApp(directory)
    app.run()


#!/usr/bin/python3
# -*- coding: UTF-8 -*-

__all__ = ['a']
__version__ = '0.1'
__author__ = 'Luis Morales' 


"""
Set up the 'meteo.in' from the files 'example_data.csv' and  'example_sim.csv' downloaded from  https://github.com/XiaoxiangGuanGFZ/GR4J/tree/main/GR4J_C/data
"""

# Import modules
import sys,os
from datetime import datetime
import csv
import pandas as pd

# Import other modules

# Function/class definition
def write_to_file(output_file, data, header=None):
    """
    Write rows (list of lists) into output_file in datetime format: YYYY-MM-DD HH:MM:SS.
    Assumes rows contain at least [year, month, day] and optionally [hour, minute, second].
    """

    #  hour  = 12
    #  minute= 0
    #  second= 0
    with open(output_file, "w", newline='') as f:
        writer = csv.writer(f)
        if header:
            writer.writerow(header)
        for idx, row1 in data.iterrows():
            print(row1)
            #y,m,d,Prec,Ep,Tavg,Tmax,Runoff
            # Pad with zeros and default missing time parts to 0
            year  = int(row1['y'])
            month = int(row1['m'])
            day   = int(row1['d'])
            tave = row1['Tavg']
            tmin = row1['Tmax']
            tmax = row1['Tmax']
            precip = row1['Prec']
            runoff = row1['Runoff']
            evapt = row1['Ep']

            values = [tave, tmin, tmax, precip, evapt, runoff]

            # Format datetime
            dt = datetime(year, month, day).strftime("%Y-%m-%d %H:%M:%S")

            writer.writerow([dt] + values)
    #header = ["datetime","tave_oC","tmin_oC","tmax_oC","precip_mm","disch_m3s"]
            #f.write(dt.strftime("%Y-%m-%d %H:%M:%S") + "\n")

def main():
    """
    """

    # Read file1
    data = pd.read_csv(file)

    # Show first 4 rows
    print(data.head())
     
    # Write into output file
    write_to_file(output_file, data, header)

if __name__ == "__main__":

    # Define input/output data
    file = 'example_data.csv'
    cols = None 
    output_file = '../meteo.in'
    header = ["datetime","tave_oC","tmin_oC","tmax_oC","precip_mm","evapt_mm","runoff_mm"]
    
    # Call main
    main()

#!/usr/bin/python3
# -*- coding: UTF-8 -*-

__all__ = ['a']
__version__ = '0.1'
__author__ = 'Luis Morales' 


"""
Set up the 'meteo.in' from the files 'data_Tavg.txt' and  'data_Tminmax.txt' downloaded from https://github.com/mxgiuliani00/hbv
"""

# Import modules
import sys,os
from datetime import datetime
import csv

# Import other modules

# Function/class definition
def read_between_labels_columns(filename, start_label, end_label, columns=None):
    """
    Read lines between start_label and end_label from a tab-separated file.
    
    Parameters
    ----------
    filename : str
        Path to the file.
    start_label : str
        Text marking the beginning of the block.
    end_label : str
        Text marking the end of the block.
    columns : list[int], optional
        List of column indices to extract (0-based). If None, all columns are returned.
    
    Returns
    -------
    list[list[str]]
        Extracted rows (each row is a list of selected column values).
    """
    rows = []
    inside_block = False

    with open(filename, "r") as f:
        for line in f:
            if start_label in line:
                inside_block = True
                continue
            if end_label in line:
                inside_block = False
                break
            if inside_block:
                parts = line.rstrip("\n").split()
                if columns is not None:
                    selected = [parts[i] for i in columns if i < len(parts)]
                else:
                    selected = parts
                rows.append(selected)

    return rows

def write_to_file(output_file, data1, data2, header=None):
    """
    Write rows (list of lists) into output_file in datetime format: YYYY-MM-DD HH:MM:SS.
    Assumes rows contain at least [year, month, day] and optionally [hour, minute, second].
    """

    basin_area = 1315.000000 # square miles
    basin_area *=2.59e+6 # to square meters
    #  hour  = 12
    #  minute= 0
    #  second= 0
    with open(output_file, "w", newline='') as f:
        writer = csv.writer(f)
        if header:
            writer.writerow(header)
        for row1, row2 in zip(data1,data2):
            # Pad with zeros and default missing time parts to 0
            year  = int(float(row1[0]))
            month = int(float(row1[1]))
            day   = int(float(row1[2]))
            #dt = datetime(year, month, day, hour, minute, second)

            tave = float(row1[5])
            tmin = float(row2[1])
            tmax = float(row2[0])
            precip = float(row1[3])
            #disch = float(row1[4])*basin_area*0.001/(24*60*60)
            disch = float(row1[4])

            values = [tave, tmin, tmax, precip, disch]

            # Format datetime
            dt = datetime(year, month, day).strftime("%Y-%m-%d %H:%M:%S")

            writer.writerow([dt] + values)
    #header = ["datetime","tave_oC","tmin_oC","tmax_oC","precip_mm","disch_m3s"]
            #f.write(dt.strftime("%Y-%m-%d %H:%M:%S") + "\n")

def main():
    """
    """

    # Read file1
    data1 = read_between_labels_columns(file1, start_label, end_label)
    for row in data1:
        print(row)
    
    # Read file2
    data2 = read_between_labels_columns(file2, start_label, end_label, columns=cols2)
    #  for row in data2:
    #      print(row)
    
    # Write into output file
    write_to_file(output_file, data1,data2, header)

if __name__ == "__main__":

    # Define input/output data
    file1 = 'data_Tavg.txt'
    file2 = 'data_Tminmax.txt'
    start_label = "<DATA_START>"
    end_label = "<DATA_END>"
    cols1 = None 
    cols2 = [5,6]
    output_file = '../meteo.in'
    header = ["datetime","tave_oC","tmin_oC","tmax_oC","precip_mm","disch_m3s"]
    
    # Call main
    main()

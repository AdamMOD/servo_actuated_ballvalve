import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import os
import glob

root = "data\\9-6-2021-testing\\"
os.chdir(root)
old_files = glob.glob('*.{}'.format("txt"))
new_files = [s.replace("txt", "csv") for s in old_files]

for f_path_old, f_path_new in zip(old_files, new_files):
    f0 = open(f_path_old,"r")
    fb = open(f_path_new,"w+")
    for c,l in enumerate(f0.readlines()):
        if c == 0:
            l_clean = "State," + l
        else:
            l_clean = l.replace("psi,", "psi")
            l_clean = l_clean.replace(", ", ",")
        fb.write(l_clean)

    f0.close()
    fb.close()

    arr = pd.read_csv(f_path_new)
    arr["Time"] = arr["Time"].str.replace(r'[^\d*\.?\d+]', '')
    arr["Command"] = arr["Command"].str.replace(r'[^\d*\.?\d+]', '')
    arr["Volts"] = arr["Volts"].str.replace(r'[^\d*\.?\d+]', '')
    arr["Psi"] = arr["Psi"].str.replace(r'[^\d*\.?\d+]', '')
    arr.to_csv(f_path_new)

import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import os
import glob
import scipy.integrate as integrate
sns.set_style("whitegrid")

root = "data\\9-6-2021-testing\\"
os.chdir(root)
datafiles = glob.glob('*.{}'.format("csv"))

runs = []
TIME = 10.0
BUCKET_MASS = .360
mass = np.array([4.955,
4.834,
4.473,
4.009,
3.168,
2.823,
2.306,
1.992,
1.468,
0.8,
0.645,
0.362])
for i,f in enumerate(datafiles):
    arr = pd.read_csv(f)
    # print(arr)
    avg = 1/10.0 * integrate.simps(arr["Psi"][arr["Time"] <= 10], arr["Time"][arr["Time"] <= 10])
    try:
        runs.append([float(f.replace(".csv","").replace("t","")), avg, mass[i]])
    except:
        pass
    print(f.replace(".csv","").replace("t",""),",",avg)

runs = np.array(runs)
cv = (runs[:,2] - BUCKET_MASS) / TIME * 60 * 0.264172 / np.sqrt(runs[:,1])
plt.scatter(runs[:,0], cv)
plt.show()
"""
fig, ax1 = plt.subplots()
ax2 = ax1.twinx()
ax1.plot(arr["Time"], arr["Psi"], label="Pressure")
ax1.hlines(avg, 0, 10, color="red")
ax2.plot(arr["Time"], arr["Command"], color="green", label="Command")
ax1.legend()
ax2.legend()
ax1.set_ylabel("Pressure (psi)")
ax2.set_ylabel("Command (0 is open)")
plt.show()
"""
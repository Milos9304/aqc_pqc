import glob, os
import numpy as np

for qubits in [9,10,11,12]:
    res = []    
    for seed in range(20):
        file=str(qubits)+"_"+str(seed)+".log"
        with open(file, 'r') as f:
            lines = f.read().splitlines()
            row=float(lines[-2].split(' ')[-1])
            res.append(row)

    np.save(str(qubits)+"_final_energies.npy", res)

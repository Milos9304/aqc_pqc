import glob, os
import numpy as np

for qubits in [9,10,11,12]:
    res = []    
    for seed in range(20):
        file=str(qubits)+"_"+str(seed)+".bkp"
        with open(file, 'r') as f:
            lines = f.read().splitlines()
            row=lines[-2][7:-1].split(' ')
            res.append(row)

    np.save(str(qubits)+"_final_params.npy", res)

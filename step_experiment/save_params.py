import glob, os
import numpy as np

res = []
for i in range(36):
    res.append([[0]*20]*20)

for steps in [5,10,15,20,25,30,35]:

    for seed in range(20):
        file="step_experiment_"+str(steps)+"/10_"+str(seed)+".bkp"
        with open(file, 'r') as f:
            lines = f.read().splitlines()
            row=list(map(float, lines[-2][7:-1].split(' ')))
            res[steps][seed]=row

np.save("final_params.npy", res)

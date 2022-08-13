import glob
import matplotlib.pyplot as plt
import numpy as np

ys = []
legends = []

for file_name in glob.glob("*.log"):
    f=open(file_name)
    legends.append(file_name)
    y=[]
    for line in f.readlines():
        eval, expec = line[:-1].split(' ')
        y.append(abs(float(eval)-float(expec)))
    ys.append(y)
    
x = np.linspace(0, 49, 50)

for i in range(len(ys)):
    
    plt.plot(x, ys[i]+0.01*i*np.ones((len(ys[i]))))

plt.show()
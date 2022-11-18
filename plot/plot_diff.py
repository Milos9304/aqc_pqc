import glob
import matplotlib.pyplot as plt
import numpy as np

ys = []
legends = []

for file_name in glob.glob("*.log"):
    f=open(file_name)
    legends.append(file_name)
    y=[]
    evals = []
    expecs = []
    for line in f.readlines():
        #print(line)
        eval, expec = line[:-1].split(' ')[:2]
        evals.append(float(eval))
        expecs.append(float(expec))
        #y.append(abs(float(eval)-float(expec)))
    
    x = np.linspace(0, len(evals)-1, len(evals))
    #ys.append(y)
    
    #for i in range(len(evals)):
    #    print(evals[i],' ',expecs[i],' ',expecs[i]-evals[i])
    #njfkdvkslnjk
    plt.plot(x, evals)
    plt.plot(x, expecs)
    plt.title(file_name)
    plt.show()
        
    
    #plt.plot(evals)
    #plt.plot(expecs)
    #plt.show()
    

print(len(ys[0]))
jk
for i in range(len(ys)):
    
    plt.plot(ys[i]+0.01*i*np.ones((len(ys[i]))), 'o')

plt.show()

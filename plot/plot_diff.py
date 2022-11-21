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
    hes = {}
    i=0
    hes_eval = False
    for line in f.readlines():
        #print(line)
        
        if line.startswith("HES: "):
            hes[i] = list(map(float, line.split(' ')[1:]))
            i+=1
            hes_eval = True
        else:
            eval, expec = line[:-1].split(' ')[:2]
            evals.append(float(eval))
            expecs.append(float(expec))
            #y.append(abs(float(eval)-float(expec)))
        
    
    x = np.linspace(0, len(evals)-1, len(evals))
    #ys.append(y)
    
    #for i in range(len(evals)):
    #    print(evals[i],' ',expecs[i],' ',expecs[i]-evals[i])
    #njfkdvkslnjk
    
    if hes_eval:

        firstHES = []
        for i in range(len(x)):
            firstHES.append(hes[i][1])
        secondHES = []
        for i in range(len(x)):
            secondHES.append(hes[i][2])
        thirdHES = []
        for i in range(len(x)):
            thirdHES.append(hes[i][3])
        fourthHES = []
        for i in range(len(x)):
            fourthHES.append(hes[i][4])
    
    plt.plot(x, evals, 'orange')
    plt.plot(x, expecs, 'blue')
    
    if hes_eval:
        plt.plot(x, firstHES, color=(1, 0, 0))
        plt.plot(x, secondHES, color=(1, 0.2, 0.2))
        plt.plot(x, thirdHES, color=(1, 0.4, 0.4))
        plt.plot(x, fourthHES, color=(1, 0.6, 0.6))
        plt.title(file_name)
    plt.show()
        
    
    #plt.plot(evals)
    #plt.plot(expecs)
    #plt.show()
    

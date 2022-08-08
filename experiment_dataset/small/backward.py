import networkx as nx
import numpy as np
from qiskit_optimization.applications import Maxcut
from qiskit_optimization.problems import QuadraticProgram
from qiskit.algorithms import NumPyMinimumEigensolver
from qiskit_optimization.algorithms import MinimumEigenOptimizer

opt_vals = []

for num_qubits in [2]:
    for seed in [1]:
        graph = nx.random_regular_graph(1, num_qubits, seed=seed)
        w = nx.to_numpy_matrix(graph, nodelist=sorted(graph.nodes()), dtype=np.intc)
        
        print(w)
        max_cut = Maxcut(w)
        qp = max_cut.to_quadratic_program()
        qubitOp, offset = qp.to_ising()
        #print("Offset:", offset)
        #print("Ising Hamiltonian:")
        #print(str(qubitOp))
        #print()      
        exact = MinimumEigenOptimizer(NumPyMinimumEigensolver())
        result = exact.solve(qp)
        print(result)
        
        with open('backward/'+str(num_qubits)+'_'+str(seed)+'.npy', 'wb') as f:
            np.save(f, w)
        
        opt_vals.append([str(num_qubits)+'_'+str(seed), result.x])

with open('backward/solutions.txt', 'w') as f:
    for o in opt_vals:
        val = 0
        binary = str(list(map(int, o[1])))[1:-1].replace(',','').replace(' ','')#[::-1]
        for i in range(len(binary)-1, -1, -1):
            val += int(binary[i])*2**i
            
        f.write(o[0]+' '+str(val)+'\n')

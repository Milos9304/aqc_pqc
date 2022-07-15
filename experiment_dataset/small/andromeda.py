import networkx as nx
import numpy as np
from qiskit_optimization.applications import Maxcut
from qiskit_optimization.problems import QuadraticProgram
from qiskit.algorithms import NumPyMinimumEigensolver
from qiskit_optimization.algorithms import MinimumEigenOptimizer

for num_qubits in [6,8]:
    for seed in [1,2,3]:
        graph = nx.random_regular_graph(3, num_qubits, seed=seed)
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
        
        #with open('andromeda/'+str(num_qubits)+'_'+str(seed)+'.npy', 'wb') as f:
        #    np.save(f, w)

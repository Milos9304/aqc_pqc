import networkx as nx
import numpy as np
from qiskit_optimization.applications import Maxcut
from qiskit.algorithms import NumPyMinimumEigensolver
from qiskit_optimization.algorithms import MinimumEigenOptimizer
import collections

opt_vals = []

#We start by defining the Pauli Matrices.
pauliz = np.array([[1, 0], [0, -1]])
paulix = np.array([[0, 1], [1, 0]])
pauliy = np.array([[0, -1j], [1j, 0]])
identity = np.array([[1, 0], [0, 1]])

def best_cost_brute(adjacency_matrix): #This function calculates the optimal cost function by brute force
    best_cost = 0
    number_of_qubits = len(adjacency_matrix)
    best_string = 0
    costs = collections.defaultdict(list)
    for b in range(2**number_of_qubits):
        x = [int(t) for t in reversed(list(bin(b)[2:].zfill(number_of_qubits)))]
        cost = 0
        for i in range(number_of_qubits):
            for j in range(number_of_qubits):
                cost += adjacency_matrix[i,j] * x[i] * (1-x[j])
        cost = np.round(cost,4)

        #x.reverse()
        costs[cost].append(x)

        if best_cost < cost:
            best_cost = cost
            best_string = x

    costs = sorted(costs.items())
    return best_cost, best_string, costs


def tensor_pauli(number_of_qubits, which_qubit, pauli_matrix): #This matrix represents a Pauli matrix acting in a single qubit in high dimensional Hilbert Spaces

    if which_qubit == 0:
        matrix = pauli_matrix
    else:
        matrix = identity

    for qubit in range(1, number_of_qubits):
        if which_qubit == qubit:
            matrix = np.kron(pauli_matrix, matrix)
        else:
            matrix = np.kron(identity, matrix)

    return matrix
def maxcut_hamiltonian(adjacency_matrix): #This functions creates the matrix representation (2^n x 2^n) of the MaxCut Hamiltonian.
    
    Hamiltonian = np.zeros((2**adjacency_matrix.shape[0], 2**adjacency_matrix.shape[0]))
    for vertex1 in range(adjacency_matrix.shape[0]):
        for vertex2 in range(adjacency_matrix.shape[0]):
            if vertex1 < vertex2:
                if adjacency_matrix[vertex1, vertex2] != 0:
                    Hamiltonian += 1/2*adjacency_matrix[vertex1, vertex2]*tensor_pauli(adjacency_matrix.shape[0], vertex1, pauliz)@tensor_pauli(adjacency_matrix.shape[0], vertex2, pauliz)

    return Hamiltonian

np.random.seed(0)
for num_qubits in [8, 9, 10]:
    for seed in range(20):
        edges = num_qubits*(num_qubits-1)/2
        graph = nx.gnm_random_graph(num_qubits, edges, seed=seed)
        #graph = nx.random_regular_graph(3, num_qubits, seed=0)
        w = nx.to_numpy_matrix(graph, nodelist=sorted(graph.nodes()))
        for i in range(num_qubits):
            for j in range(num_qubits):
                if i<j:
                    if w[i,j] != 0:
                        w[i,j] = round(np.random.uniform(0,1), 2)
                        w[j,i] = w[i,j]

        max_cut = Maxcut(w)
        qp = max_cut.to_quadratic_program()
        qubitOp, offset = qp.to_ising()
        print("Offset:", offset)
            #print("Ising Hamiltonian:")
            #print(str(qubitOp))
            #print()      
        exact = MinimumEigenOptimizer(NumPyMinimumEigensolver())
        result = exact.solve(qp)
        print(result)
        best_cost, best_string, costs = best_cost_brute(w)
            #print(costs)
        print(f'For the given instance the optimal cost is {best_cost} and the bitstrings corresponding to that are {costs[-1][1]}')
        
                
        with open('kol_8_10/'+str(num_qubits)+'_'+str(seed)+'.npy', 'wb') as f:
            np.save(f, w)
        
        opt_vals.append([str(num_qubits)+'_'+str(seed), costs[-1][1]])
        print(opt_vals)

with open('kol_8_10/solutions.txt', 'w') as f:
    for o in opt_vals:
        f.write(o[0])
        for cost in o[1]:
            val = 0
            binary = str(list(map(int, cost)))[1:-1].replace(',','').replace(' ','')[::-1]
            for i in range(len(binary)-1, -1, -1):
                val += int(binary[i])*2**i
            f.write(' '+str(val))
        f.write('\n')

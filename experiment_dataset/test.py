from qiskit.visualization import *
import numpy as np
import matplotlib.pyplot as plt
from quantum_circuit import QCir
from qiskit.quantum_info import Statevector
import scipy.optimize as optimize
import networkx as nx
import collections
from qiskit.aqua.operators.gradients import Hessian
from qiskit.quantum_info.operators import Operator

pauliz = np.array([[1, 0], [0, -1]])
paulix = np.array([[0, 1], [1, 0]])
pauliy = np.array([[0, -1j], [1j, 0]])
identity = np.array([[1, 0], [0, 1]])

steps = 50 #Choose number of steps to interpolate from initial to final Hamiltonian
connectivity = 'nearest-neighbors' #This is the connectivity of the non-parameterized gates in the Hardware Efficient ansatz

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

def initial_hamiltonian(number_of_qubits): #Here we define the initial Hamiltonian which we choose it to be -sigma_x for all qubits.

    initial_ham = np.zeros((2**number_of_qubits, 2**number_of_qubits))
    for qubit in range(number_of_qubits):
        initial_ham -= tensor_pauli(number_of_qubits, qubit, paulix)

    return initial_ham

def maxcut_hamiltonian(number_of_qubits, adjacency_matrix): #This functions creates the matrix representation (2^n x 2^n) of the MaxCut Hamiltonian.

    Hamiltonian = np.zeros((2**number_of_qubits, 2**number_of_qubits))
    for vertex1 in range(number_of_qubits):
        for vertex2 in range(number_of_qubits):
            if vertex1 < vertex2:
                if w[vertex1, vertex2] != 0:
                    Hamiltonian += 1/2*np.dot(tensor_pauli(number_of_qubits, vertex1, pauliz), tensor_pauli(number_of_qubits, vertex2, pauliz))

    return Hamiltonian


for num_qubits in [6,8]:
    for seed in [1,2,3]:
        graph = nx.random_regular_graph(3, num_qubits, seed=seed)
        w = nx.to_numpy_matrix(graph, nodelist=sorted(graph.nodes()), dtype=np.intc)

        H_0 = initial_hamiltonian(num_qubits)
        H_1 = maxcut_hamiltonian(num_qubits, w)
        
        print(H_0)
        print(H_1)
        

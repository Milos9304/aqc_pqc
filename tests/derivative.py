import numpy as np
from sympy import *
from math import cos, sin
from cmath import pi, sqrt
from numpy import linalg as LA
from sympy.physics.quantum import TensorProduct
import sympy

init_printing(use_unicode=True)

def ry(theta):
    return Matrix([[sympy.cos(theta/2), -sympy.sin(theta/2)],
                  [sympy.sin(theta/2), sympy.cos(theta/2)]])

theta_00 = Symbol('t00')
theta_01 = Symbol('t01')
theta_10 = Symbol('t10')
theta_11 = Symbol('t11')

zero = Matrix([0]*4)
zero[0]=1.

ry_00 = ry(theta_00)
ry_01 = ry(theta_01)
ry_10 = ry(theta_10)
ry_11 = ry(theta_11)

t_0 = TensorProduct(ry_01, ry_00)
t_1 = TensorProduct(ry_11, ry_10)

#print(t_0)
print(t_1.evalf(subs={theta_00: 0, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))
psi=(t_0*t_1)*zero

print(psi.evalf(subs={theta_00: 0, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))

h = Matrix([[0,0,0,-1],
               [0,0,-1,0],
               [0,-1,0,0],
               [-1,0,0,0]])

e=(psi.T*h*psi)[0]

dx=diff(e, theta_00)
print(dx.evalf(subs={theta_00: 0, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))
dx=diff(e, theta_01)
print(dx.evalf(subs={theta_00: 0, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))
dx=diff(e, theta_10)
print(dx.evalf(subs={theta_00: 0, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))
dx=diff(e, theta_11)
print(dx.evalf(subs={theta_00: 0, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))

print(psi.evalf(subs={theta_00: pi/2, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))
a=e.evalf(subs={theta_00: 0+pi/2, theta_01: 0, theta_10: pi/2, theta_11: pi/2})
b=e.evalf(subs={theta_00: 0-pi/2, theta_01: 0, theta_10: pi/2, theta_11: pi/2})
print(a, ' ', b, ' ', 0.5*(a-b))
print((t_0*t_1).evalf(subs={theta_00: pi/2, theta_01: 0, theta_10: pi/2, theta_11: pi/2}))
#w, v = LA.eig(h)
#print(w)
#print(v)"""
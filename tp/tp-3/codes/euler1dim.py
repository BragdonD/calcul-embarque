import numpy as np
import matplotlib.pyplot as plt
import math 

def euler_y(x,y):
    return -y+math.exp(-x)    #ici l equa differentielle etudiée


def euler_resolution(h,y,x):
    return y+h*euler_y(x,y)

h=.01
y0=float(input('y initial : ')) #prendre 0 comme valeur init de y pour cette equa diff ou modif solution exacte

nb_points=1000

t=np.linspace(0,nb_points,nb_points)
y=np.linspace(0,0,nb_points)
solution=np.linspace(0,0,nb_points)
x=np.linspace(0,nb_points*h,nb_points)

y[0]=y0



for i in range(1,nb_points):
    y[i]=euler_resolution(h,y[i-1],x[i])
    solution[i]=x[i]*math.exp(-x[i])

plt.plot(t,y,solution)
plt.show() 




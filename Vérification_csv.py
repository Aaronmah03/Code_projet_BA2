#ATTENTION TJRS RETIRER LES 'XY' DE LA PREMIERE LIGNE DU CSV
import matplotlib.pyplot as plt
import numpy as np
nom=input()
def neg(a):
    if a=='B' or a=='H':
        neg=1
    else :
        neg=-1
    return neg
x,y= np.loadtxt('csv_projet/'+nom+'.csv', delimiter=',', unpack=True)
plt.plot(x,neg(nom)*y, marker='.',markersize=1)
plt.title('Vérification du CSV')
plt.show()

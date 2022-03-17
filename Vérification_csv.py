#ATTENTION TJRS RETIRER LES 'XY' DE LA PREMIERE LIGNE DU CSV
import matplotlib.pyplot as plt
import numpy as np
x,y= np.loadtxt('fichier.csv', delimiter=',', unpack=True)
plt.plot(x,y, marker='.',markersize=1)
plt.title('Vérification du CSV')
plt.show()
\end{minted}
Et celui écrit en matlab;
\begin{minted}{matlab}
data = csvread('fichier.csv'); % Read the data
t = data(:,1) ; 
x = data(:,2) ; 
data
figure('NumberTitle','off','Name','Vérification csv','Color','w')
plot(t,x,marker='.',markersize=8)
xlabel('csv')

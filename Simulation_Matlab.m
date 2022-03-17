clear
t = linspace(0,2.02*pi,101); 
l = 1.5; %largeur tableau (x)
L = 2;  %longueur tableau (y)
r = 0.3;   %rayon du cercle
R = 0.015; %rayon des engrenages
K1 = 60;  %gain moteur1  [K]=(9.6/255)*Volt/rad
K2 = 60;  %gain moteur2

X= r*cos(0) + l/2; %point de départ en x
Y = r*sin(0) + L/2; %"  " en y  
Xerreur = r*cos(0) + l/2; 
Yerreur = r*sin(0) + L/2;
d1 = 0;
d2 = 0;
a1 = 0;
a2 = 0;
a1erreur = 0;
a2erreur = 0;
w1 = 0;
w2 = 0;

%% Tracée de la figure

for k=2:length(t)  
    clf
    e1 = (rand-1/2)*2;  %Erreur moteur1 [-1;1]
    e2 = (rand-1/2)*2;  %Erreur moteur2
    x = r*cos(t) + l/2;  % Position en x du stylo (plotter) 
    y = r*sin(t) + L/2;  % Position en y du stylo (plotter)
    x_k2= x(k);  %releve le point x au 'temps' précis k (dc pour k=4 on a 
    \\ %t=4*(2.01*pi/50))
    y_k2 = y(k);
    x_k = x(k-1); %releve le point x au à partir de k=1 (utile pour calculer 
    \\ %l'acrroissement des cordes)
    y_k = y(k-1);
    d1 = sqrt(x_k^2 + y_k^2);  %Longueur corde 1 
    d2 = sqrt((l-x_k)^2 + y_k^2); %Longueur corde 2 
    %cosO = x_k/d1; %Angle theta entre la corde et une droite horizontale 
    %sinO = y_k/d1; 
    %cosO2 = (l-x_k)/d2; %Angle theta 2 
    %sinO2 = y_k/d2;
    delta_d1 = sqrt(x_k2^2 + y_k2^2)- d1;  %d1'-d1 (d1'= longueur corde en k et 
    \\ % d = '' en k-1)  
    delta_d2 = sqrt((l-x_k2)^2 + y_k2^2) - d2; %d2'-d2

    a1erreur = delta_d1/R + e1;  %position moteur1
    a2erreur = delta_d2/R + e2;   % "  " mot2
    d1erreur = a1erreur*R + d1;  %Longueur corde1 avec l'erreur
    d2erreur = a2erreur*R + d2;  %Longuer corde2 avec l'erreur
    cosO = (l^2 + d1erreur^2 - d2erreur^2)/(2*d1erreur*l); %Angle theta entre la 
    \\ %corde1 et l'horizontale
    sinO = sqrt(1 - (cosO)^2);
    
    x_kerreur = d1erreur*cosO;  %Position en x dû à l'erreur
    y_kerreur = d1erreur*sinO;  %Position en y dû à l'erreur

    V1 = K1*e1 ;  %Tension appliqué mot1 avec régulation P pour compenser l'erreur
    V2 = K2*e2;  %Tension appliqué mot2 avec régulation P
   
    w1 = 0.1639*V1; %Vitesse rotation mot1 
    w2 = 0.1533*V2; %Vitesse rotation mot2
    
    a1 = w1*0.1 ;  %Ouverture angulaire avec correction du mot1 
    a2 = w2*0.1 ;  %0.1 = temps pris pour parcourir pi/50 (pr tracer un cercle de 
    \\ %r=0.3m en 10sec)
    
    d1regul = - a1*R + d1erreur;  %Longueur corde1 corrigé 
    d2regul = - a2*R + d2erreur;  %Longueur corde2 corrigé 
    cosO = (l^2 + d1regul^2 - d2regul^2)/(2*d1regul*l);
    sinO = sqrt(1 - (cosO)^2);

    x_k = d1regul*cosO;  %Position en x avec approximation des angles
    y_k = d1regul*sinO;  %Position en y ''
   
   
    %Mise en place du graphe 
    ha = gca;     %permet de manipuler les propriétés des axes (échelle,direct°,etc.)
    set(ha,'ydir','reverse')   %inverse axe y 
    xlabel('x (distance en m)'); ylabel('y (distance en m)')
    grid on  %forme un quadrillage pour mieux répérer les position 
    box on %referme le graphe
    axis equal  %axe à la même échelle
    title('Simulation')

    text(0.1,0.3,['a1erreur=',num2str(a1erreur)]) 
    text(0.1,0.4,['a1=',num2str(a1)])
    text(0.1,0.5,['w1=',num2str(w1)])

    % Trace les 2 cordes en suivant le point en tt temps
    hold on %Pour tracer plusieurs plot 
    line([0,x_k],[0,y_k],'Color','k','Marker','.',Markersize=14, Linewidth=2) 
    line([1.5,x_k],[0,y_k], 'Color','k','Marker','.',Markersize=14, Linewidth=2)
    line([0,x_kerreur],[0,y_kerreur],'Color','r','Marker','.',Markersize=14,
    Linewidth=1) 
    line([1.5,x_kerreur],[0,y_kerreur], 'Color','r','Marker','.',Markersize=14, 
    Linewidth=1)

    %Dessine le cercle 
    X = [X x_k];  
    Y = [Y y_k];   
    Xerreur = [Xerreur x_kerreur];  
    Yerreur = [Yerreur y_kerreur]; 
    plot(Xerreur, Yerreur, 'r',Linewidth=1) 
    hold on 
    plot(X,Y,'b',Linewidth=1) 
    
    drawnow %oblige matlab à montrer la formation du dessin 
end 

%legend('','Cordes','Cordes','','Cercle avec erreur','Cercle régulé','Location',
\\ %'best') %choisis l'endroit neutre pr mettre la legende


%%%Sources https://www.youtube.com/watch?v=gDmpqn92s5U
%%%https://www.youtube.com/watch?v=3I1_5M7Okqo
%%%https://www.youtube.com/watch?v=sF_sjFqNFUk(with simulink

#include <Servo.h> // Bibliothèque sevomoteur

// Pins moteur 1
#define DIR1 2 
#define PWM1 3
#define CHA1 9
#define CHB1 10

// Pins moteur 2
#define DIR2 4
#define PWM2 5
#define CHA2 6
#define CHB2 7

#define SER 12 // Pin servomoteur

#define PI 3.1415926535897932384626433832795 // Pi, 31 decimales

Servo servo1; // Creation du servomoteur
int a = 90; // Angle de rotation du servomoteur

float L = 144; // Distance entre les moteurs (en cm)
float H = 125; // Hauteur du support
float R = 1.75; // Rayon de l'engrenage
float x0; // Point de depart au centre du tableau (au milieu du tableau)
float y0;
float x1; // Posisitons du point suivant
float y1;

float V_H1 = 10; // Valeur minimale de la fonction analogWrite necessaire pour faire tourner les moteurs
float V_T1 = 10;
float V_H2 = 10;
float V_T2 = 10;
float u_1; // Tensions a appliquer aux moteurs
float u_2;
float u_max = 70;

float delai_regulation = 20; // Temps pour la regulation (en ms)
float K1 = 5; // Coefficient necessaire pour la regulation
float K2 = 5;
int t; // Temps
int dt; // Intervalle de temps
char state1; // Etats des moteurs (HIGH ou LOW)
char state2;

float pos_1 = 0; // Position moteur initiale
float pos_2 = 0;
float pos1_T = 0; // Position moteur initiale théorique
float pos2_T = 0;
float err_1 = 0; // Erreur initiale
float err_2 = 0;
float derr = 10; // Différence acceptable sur les erreurs
int m;


float longueur_1(float x, float y) { 
  // Fonction qui calcule la longeur de la courroie gauche pour une position XY
  float l1 = sqrt(pow(x, 2) + pow(y, 2));
  return l1;
}


float longueur_2(float x, float y) {
  // Fonction qui calcule la longeur de la courroie droite pour une position XY
  float l2 = sqrt(pow(L - x, 2) + pow(y, 2));
  return l2;
}


float delta(float l1, float l2) {
  // Fonction calculant la difference de longeur de la courroie entre 2 points
  float dl = l2 - l1;
  return dl;
}


void pulses_1() {
  // Fonction qui modifie la position du moteur 1 des que l'on recoit un signal de l'encodeur
  int v1_0 = digitalRead(CHA1);
  int v1_1 = digitalRead(CHB1);
  if (v1_0 == v1_1) {
    pos_1--;
  } else {
    pos_1++;
  }
}


void pulses_2() {
  // Fonction qui modifie la position du moteur 2 des qu'on recoit un signal de l'encodeur
  int v2_0 = digitalRead(CHA2);
  int v2_1 = digitalRead(CHB2);
  if (v2_0 == v2_1) {
    pos_2--;
  } else {
    pos_2++;
  }
}


float calcul_angle_1(float dl1) {
  // Calcule la difference d'angle pour le moteur 1 pour une difference de longueur de la courroie 1 (en degres)
  float ds1 = (dl1 * 180) / (R * PI);
  return ds1;
}


float calcul_angle_2(float dl2) {
  // Calcule la difference d'angle pour le moteur 2 pour une difference de longueur de la courroie 2 (en degres)
  float ds2 = (-dl2 * 180) / (R * PI); // Signe - car le moteur tourne dans le sens anti-horlogique quand la longueur de la courroie augmente
  return ds2;
}


void regulation(float e1, float e2) {
  // Fonction qui s'occupe de reguler les positions des moteurs en fonction des erreurs de position 
  if (e1 > 0) {
    state1 = HIGH;
    u_1 = V_H1 + K1 * e1;
  }
  else if (e1 <= 0) {
    state1 = LOW;
    u_1 = V_T1 - K2 * e1;
  }
  if (e2 > 0) {
    state2 = HIGH;
    u_2 = V_H2 + K2 * e2;
  }
  else if (e2 <= 0) {
    state2 = LOW;
    u_2 = V_T2 - K1 * e2;    
  } 
  if (u_1 > u_max) {
    u_1 = u_max;
  }
  if (u_2 > u_max) {
    u_2 = u_max;
  }
  digitalWrite(DIR1, state1);
  digitalWrite(DIR2, state2);
  analogWrite(PWM1, u_1);
  analogWrite(PWM2, u_2);
  delay(delai_regulation);
}


void tout(float x0, float x1, float y0, float y1) {
  // Fonction qui fait tout
  // Calcul des longueurs 
  float L1_0 = longueur_1(x0, y0); 
  float L1_1 = longueur_1(x1, y1);
  float dc1 = delta(L1_0, L1_1);
  float L2_0 = longueur_2(x0, y0);
  float L2_1 = longueur_2(x1, y1);
  float dc2 = delta(L2_0, L2_1);
            
  // Partie regulation
  pos1_T +=  calcul_angle_1(dc1);
  pos2_T +=  calcul_angle_2(dc2);
  float pos1_E = pos_1;
  float pos2_E = pos_2;
  err_1 = pos1_T - pos1_E;
  Serial.println(err_1); 
  err_2 = pos2_T - pos2_E;
  Serial.println(err_2);
  regulation(err_1, err_2);
  /*while(abs(err_1 > derr) || abs(err_2 > derr) || m < 3) {
    regulation(err_1, err_2);
    err_1 = pos1_T - pos1_E;
    err_2 = pos2_T - pos2_E;     
    m++; 
  }*/
}



void setup() {
  Serial.begin(9600); 
  // Moteur 1
  pinMode(CHA1, INPUT_PULLUP);
  pinMode(CHB1, INPUT);
  pinMode(DIR1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CHA1), pulses_1, RISING);
  // Moteur 2
  pinMode(CHA2, INPUT_PULLUP);
  pinMode(CHB2, INPUT);
  pinMode(DIR2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CHA2), pulses_2, RISING);
  // Servomoteur
  servo1.attach(SER);
  // Communication arduino
  Serial.setTimeout(100);
}



void loop() {
  x0 = 68;
  y0 = 91;  
  while (!Serial.available()){
    for (int i = 0; i < 2; i++) {
      if (i == 0) {
        x1 = Serial.readString().toInt();
        //Serial.print("x="); 
        //Serial.println(x1);
      }
      //delay(20);
      else if (i == 1) {
        y1 = Serial.readString().toInt();
        //Serial.print("y="); 
        //Serial.println(y1);
        tout(x0, x1, y0, y1);
      }  
      x0 = x1;
      y0 = y1;
    }
  //analogWrite(PWM1,0);
  //analogWrite(PWM2,0); 
  }
}

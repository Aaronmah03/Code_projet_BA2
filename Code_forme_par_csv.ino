float x;
float y;
float servo;

#include "Arduino.h"

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
      while (!Serial.available());
      
      
              x = Serial.readString().toFloat();  //coord x
              Serial.print("x="); 
              Serial.println(x);
              delay(20);
              y = Serial.readString().toFloat(); //coord y 
              Serial.print("y="); 
              Serial.println(y);
              delay(20); 
              servo = Serial.readString().toFloat(); //Position servomoteur 
              Serial.print("PosServo=");
              Serial.println(servo);
      }





//https://create.arduino.cc/projecthub/ansh2919/serial-communication-between-python-and-arduino-e7cce0

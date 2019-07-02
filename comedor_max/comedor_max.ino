#include "Wire.h"  

//pin para chichara
int ledPIN = 13;

int IN3 = 4; 
int IN4 = 5;

int count=0;

void setup(){

  Wire.begin();
  Serial.begin(9600);

  //chichara
  pinMode(ledPIN , OUTPUT); 
  
  //motor
  pinMode (IN4, OUTPUT);    // Input4 conectada al pin 4 
  pinMode (IN3, OUTPUT);    // Input3 conectada al pin 5
  
}


void loop(){

 if(count==0){
  getChichara();
  ejecutar();
  count++;
 }
 
}


void ejecutar(){
    GiraOn();
    GiraOff();
}
  
void GiraOn(){
   // Motor gira en un sentido
  digitalWrite (IN4, HIGH);
  digitalWrite (IN3, LOW); 
  delay(19000);
}

void GiraOff(){
  // Motor no gira
  digitalWrite (IN4, LOW); 
}

void getChichara(){
  digitalWrite(ledPIN , HIGH);   // poner el Pin en HIGH
  delay(15000);                   // esperar un segundo
  digitalWrite(ledPIN , LOW);    // poner el Pin en LOW
  delay(500);  
}

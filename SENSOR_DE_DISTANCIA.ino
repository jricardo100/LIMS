/*
  Este programa faz com o Arduino meça a distância
  em cm, e para cada distância diferente ative
  LEDs indicativos de zona de alerta
*/

long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);               
  digitalWrite(triggerPin, HIGH);     // Põe o trigger em HIGH
  delayMicroseconds(10);              // Mantém o trigger em HIGH por 10 microsegundos
  digitalWrite(triggerPin, LOW);      // Põe o trigger em LOW
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);      // Lê o pino echo e retorna o tempo de retorno da onda em microsegundos
}

void setup(){
  Serial.begin(9600);
  pinMode(10, OUTPUT);
}

void loop(){
  Serial.println(0.01723 * readUltrasonicDistance(7, 6));
  if (0.01723 * readUltrasonicDistance(7, 6) < 50) {
    digitalWrite(10, HIGH);
  } 
  else {digitalWrite(10, LOW);}
  delay(1000); // Delay a little bit to improve simulation performance
}

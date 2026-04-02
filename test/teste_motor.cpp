#include <Arduino.h>

// Teste de diagnóstico para a ponte H
// Pinos do motor
int IN1 = 3;
int IN2 = 2;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  Serial.println("EXECUTANDO TESTE DA PONTE H");
  delay(1000);
}

void loop() {
  // Teste 1: IN1 HIGH, IN2 LOW
  Serial.println("\nTeste 1 - IN1=HIGH, IN2=LOW");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(3000);
  
  // Teste 2: IN1 LOW, IN2 HIGH
  Serial.println("Teste 2 - IN1=LOW, IN2=HIGH");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(3000);
  
  // Parar
  Serial.println("Parando motor...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(5000);
}

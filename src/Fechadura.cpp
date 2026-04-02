#include "Fechadura.h"
#include <Arduino.h>

// Método construtor
Fechadura::Fechadura(int in1, int in2, int buzzer)
{
    this->in1 = in1;
    this->in2 = in2;
    this->buzzer = buzzer;
    senha = PASS;
    authenticated = false;
}

// Metodos públicos
void Fechadura::begin()
{
    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
    pinMode(this->buzzer, OUTPUT);
    digitalWrite(this->buzzer, HIGH);
    Serial.println("Fechadura pronta!");
}

void Fechadura::trancar()
{
    motorIrParaFrente();
    delay(5000);
    motorParar();
}

void Fechadura::destrancar()
{
    motorIrParaTras();
    delay(5000);
    motorParar();
}

bool Fechadura::autenticar(String senha)
{
    if(senha.length() != 4)
    {
        return false;
    }
    return senha == this->senha;
}

bool Fechadura::statusDeAuth()
{
    return authenticated;
}

void Fechadura::mudarStatusDeAuth()
{
    authenticated = !authenticated;
}

// Métodos privados
void Fechadura::motorIrParaFrente()
{
    digitalWrite(this->in1, HIGH);
    digitalWrite(this->in2, LOW);
}

void Fechadura::motorIrParaTras()
{
    digitalWrite(this->in1, LOW);
    digitalWrite(this->in2, HIGH);
}

void Fechadura::motorParar()
{
    digitalWrite(this->in1, LOW);
    digitalWrite(this->in2, LOW);
}

void Fechadura::biparBuzzer() {
    digitalWrite(this->buzzer, LOW);
    delay(100);
    digitalWrite(this->buzzer, HIGH);
  }
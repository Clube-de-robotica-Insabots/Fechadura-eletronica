#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int in1, int in2)
{
    this->in1 = in1;
    this->in2 = in2;
}

void Motor::begin()
{
    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
}

void Motor::Frente(){
    digitalWrite(this->in1, HIGH);
    digitalWrite(this->in2, LOW);
}

void Motor::Tras(){
    digitalWrite(this->in1, LOW);
    digitalWrite(this->in2, HIGH);
}

void Motor::Parar(){
    digitalWrite(this->in1, LOW);
    digitalWrite(this->in2, LOW);
}


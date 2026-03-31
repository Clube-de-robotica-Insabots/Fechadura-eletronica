#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int in1, int in2)
{
    In1 = in1;
    In2 = in2;
    pinMode(In1, OUTPUT);
    pinMode(In2, OUTPUT);
}

void Motor::Frente(){
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
}

void Motor::Tras(){
    digitalWrite(In1, LOW);
    digitalWrite(In2, HIGH);
}

void Motor::Parar(){
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);
}


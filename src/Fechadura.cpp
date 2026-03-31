#include "Fechadura.h"
#include "Motor.h"
#include <Arduino.h>


Fechadura::Fechadura(int in1, int in2) : motor(in1, in2)
{
    senha = PASS;
    authenticated = false;
}

void Fechadura::trancar()
{
    motor.Frente();
    delay(1000);
    motor.Parar();
}

void Fechadura::destrancar()
{
    motor.Tras();
    delay(5000);
    motor.Parar();
}

bool Fechadura::autenticar(String senha)
{
    if(senha.length() != 3)
    {
        return false;
    }
    return senha == this->senha;
}

bool Fechadura::status()
{
    return authenticated;
}

void Fechadura::mudarStatus()
{
    authenticated = !authenticated;
}
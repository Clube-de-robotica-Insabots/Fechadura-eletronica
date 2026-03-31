#include "Fechadura.h"
#include "Motor.h"
#include "Arduino.h"


Fechadura::Fechadura(int in1, int in2) : motor(in1, in2)
{
    senha = PASS
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
    delay(1000);
    motor.Parar();
}

bool Fechadura::autenticar(String senha)
{
    if (senha.length() == 4 && senha == this->senha)
    {
        authenticated = true;
        return true;
    }
    else if (senha.length() < 4)
    {

    }
    else
    {
        authenticated = false;
        return false;
    }
}

bool Fechadura::status()
{
    return authenticated;
}

void Fechadura::mudarStatuts()
{
    authenticated = !authenticated;
}
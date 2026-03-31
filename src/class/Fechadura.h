#ifndef FECHADURA_H
#define FECHADURA_H

#include <Arduino.h>
#include "Motor.h"

class Fechadura
{
private:
    bool authenticated = false;
    bool trancada = true;
    Motor motor;
    String senha = "1234";

public:
    Fechadura(int in1, int in2) : motor(in1, in2) {}
    void trancar();
    void destrancar();
    bool autenticar(String senha);
    bool status();
    void mudarStatuts();
};

#endif
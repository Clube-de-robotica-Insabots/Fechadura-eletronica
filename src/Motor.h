#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor
{
private:
    int In1;
    int In2;
public:
    Motor(int in1, int in2);
    void Frente();
    void Tras();
    void Parar();
};

#endif
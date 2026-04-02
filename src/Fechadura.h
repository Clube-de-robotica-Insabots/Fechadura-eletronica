#ifndef FECHADURA_H
#define FECHADURA_H

#include <Arduino.h>

class Fechadura
{
private:
    int in1;
    int in2;
    bool authenticated;
    String senha;
    void motorIrParaFrente();
    void motorIrParaTras();
    void motorParar();

public:
    Fechadura(int in1, int in2);
    void begin();
    void trancar();
    void destrancar();
    bool autenticar(String senha);
    bool statusDeAuth();
    void mudarStatusDeAuth();
};

#endif
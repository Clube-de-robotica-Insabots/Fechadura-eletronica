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
    int buzzer;
    void motorIrParaFrente();
    void motorIrParaTras();
    void motorParar();

public:
    Fechadura(int in1, int in2, int buzzer);
    void begin();
    void trancar();
    void destrancar();
    bool autenticar(String senha);
    bool statusDaAutenticacao();
    void mudarStatusDeAuth(bool status);
    void biparBuzzer();
};

#endif
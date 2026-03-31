#include <Arduino.h>
#include "class/Fechadura.h"
int pin1 = 8;
int pin2 = 9;
Fechadura fechadura(pin1, pin2);
int botao = 5;
String senhaEscrita;

void setup() {
  Serial.begin(9600);
  pinMode(botao, INPUT);
  delay(2000);
  fechadura.trancar();
}

void loop() {
  // Ler a senha do usuário
  scanf("%s", senhaEscrita);
  
  // Verificar o estado do botão e da fechadura para trancar ou destrancar pelo lado de dentro
  if (digitalRead(botao) == HIGH && !fechadura.status())
  {
    fechadura.destrancar();
    fechadura.mudarStatuts();
  }
  else if (digitalRead(botao) == HIGH && fechadura.status())
  {
    fechadura.trancar();
    fechadura.mudarStatuts();
  }

  // Verificar a senha para trancar ou destrancar pelo lado de fora
  if (fechadura.autenticar(senhaEscrita))
  {
    fechadura.destrancar();
    Serial.println("Senha correta!");
  }
  else
  {
    Serial.println("Senha incorreta!");
  }
  }
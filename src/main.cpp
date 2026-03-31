#include <Arduino.h>
#include <Keypad.h>
#include "Fechadura.h"
int pin1 = 2;
int pin2 = 3;
Fechadura fechadura(pin1, pin2);
int botao = 4;
String senhaEscrita;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {5, 6, 7, 8};
byte colPins[COLS] = {9, 10, 11};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(botao, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // Verificar o estado do botão e da fechadura para trancar ou destrancar pelo lado de dentro
  if (digitalRead(botao) == LOW  && fechadura.status() == false)
  { 
    Serial.println("Destrancando...");
    fechadura.destrancar();
    fechadura.mudarStatus();
    delay(2000);
  }
  else if (digitalRead(botao) == LOW && fechadura.status() == true)
  {
    Serial.println("Trancando...");
    fechadura.trancar();
    fechadura.mudarStatus();
    delay(2000);
  }

  // Verificar a senha para destrancar pelo lado de fora, e verfica se o botão foi pressionado para destrancar
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (fechadura.autenticar(senhaEscrita))
        {
          Serial.println("Senha correta!");
          fechadura.mudarStatus();
          fechadura.destrancar();
        }

      else
        {
          Serial.println("Senha incorreta!");
        }
        senhaEscrita = "";
    }
    else if (key == '*') {
      senhaEscrita = "";
      Serial.println("Senha limpa!");
    }
    else {
      delay(200);
      senhaEscrita += key;
      Serial.print(senhaEscrita);
      Serial.println("\n");
    }
  }
}

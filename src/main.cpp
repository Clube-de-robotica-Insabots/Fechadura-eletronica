#include <Arduino.h>
#include <Keypad.h>
#include "Fechadura.h"

//Definição dos pinos
int pin1 = 2;
int pin2 = 3;
int botao = 4;
//Instancia da fechadura
Fechadura fechadura(pin1, pin2);

String senhaDigitada;

//Configuração do teclado matricial
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

//Função de configuração do Arduino e inicialização dos pinos
void setup() {
  Serial.begin(9600);
  fechadura.begin();
  pinMode(botao, INPUT_PULLUP);
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

  // Verificar a senha para destrancar pelo lado de fora
  char key = keypad.getKey();
  if (key) {
    // Verificar se a tecla pressionada é '#' para validar a senha ou '*' para limpar a senha digitada
    if (key == '#') {
      if (fechadura.autenticar(senhaDigitada))
        {
          Serial.println("Senha correta!");
          fechadura.mudarStatus();
          fechadura.destrancar();
        }

      else
        {
          Serial.println("Senha incorreta!");
        }
        senhaDigitada = "";
    }
    else if (key == '*') {
      senhaDigitada = "";
      Serial.println("Senha limpa!");
    }
    // Se for um número, adicionar à senha digitada
    else {
      delay(200);
      senhaDigitada += key;
      Serial.print(senhaDigitada);
      Serial.println("\n");
    }
  }
}

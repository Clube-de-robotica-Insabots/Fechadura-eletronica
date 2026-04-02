#include <Arduino.h>
#include <Keypad.h>
#include "Fechadura.h"

//Definição dos pinos
int pin1 = 3;
int pin2 = 2;
int botao = 4;
int buzzer = 12;
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
  pinMode(botao, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

void loop() {
  // Verificar o estado do botão e da fechadura para trancar ou destrancar pelo lado de dentro
  if (digitalRead(botao) == HIGH  && fechadura.statusDeAuth() == false)
  { 
    delay(50);
    Serial.println("Destrancar...");
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    fechadura.destrancar();
    fechadura.mudarStatusDeAuth();
    while (digitalRead(botao) == HIGH);
    delay(50);
  }
  else if (digitalRead(botao) == HIGH && fechadura.statusDeAuth() == true)
  {
    delay(50);
    Serial.println("Trancar...");
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    fechadura.trancar();
    fechadura.mudarStatusDeAuth();
    while (digitalRead(botao) == HIGH);
    delay(50);
  }

  // Verificar a senha para destrancar pelo lado de fora
  char key = keypad.getKey();
  if (key && fechadura.statusDeAuth() == false) {
    // Verificar se a tecla pressionada é '#' para validar a senha ou '*' para limpar a senha digitada
    if (key == '#') {
      if (fechadura.autenticar(senhaDigitada))
        {
          Serial.println("Senha correta!");
          Serial.println("Destrancando...");
          fechadura.mudarStatusDeAuth();
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
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      Serial.println(senhaDigitada);
    }
  }

  else if(fechadura.statusDeAuth() == true){
    Serial.println("Usuário já autenticado");
    if (key == '#'){
      fechadura.destrancar();
      fechadura.mudarStatusDeAuth();
    }
  }


}

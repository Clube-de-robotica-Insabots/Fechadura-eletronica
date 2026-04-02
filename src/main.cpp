#include <Arduino.h>
#include <Keypad.h>
#include "Fechadura.h"

//Definição dos pinos

int pin1 = 3;
int pin2 = 2;
int botao = 4;
int buzzer = 12;

//Instancia da fechadura
Fechadura fechadura(pin1, pin2, buzzer);

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

// Funções para verificar o teclado e o botão

void verificarTeclado(){
  // Verificar a senha para destrancar pelo lado de fora
  char key = keypad.getKey();
  if (key && fechadura.statusDaAutenticacao() == false) {
    // Verificar se a tecla pressionada é '#' para validar a senha ou '*' para limpar a senha digitada
    switch (key){
      case '#':
        if (fechadura.autenticar(senhaDigitada)) {
          Serial.println("Senha correta! Destrancando...");
          fechadura.mudarStatusDeAuth();
          fechadura.destrancar();
        }
        else {
          Serial.println("Senha incorreta! Tente novamente.");
          fechadura.biparBuzzer();
        }
        senhaDigitada = "";
        break;

      case '*':
        senhaDigitada = "";
        Serial.println("Senha limpa. Digite novamente.");
        break;

      default:
        senhaDigitada += key;
        fechadura.biparBuzzer();
        Serial.println(senhaDigitada);
        break;
    }
  }

  else {
    if (key == '#'){
      fechadura.destrancar();
      fechadura.mudarStatusDeAuth();
    }
  }
}
void verificarBotao(){
  // Verificar o estado do botão e da fechadura para trancar ou destrancar pelo lado de dentro
  if (digitalRead(botao) == HIGH  && fechadura.statusDaAutenticacao() == false){ 
    delay(50);
    Serial.println("Destrancar...");
    fechadura.biparBuzzer();
    fechadura.destrancar();
    fechadura.mudarStatusDeAuth();
    while (digitalRead(botao) == HIGH);
    delay(50);
  }
  else if (digitalRead(botao) == HIGH && fechadura.statusDaAutenticacao() == true){
    delay(50);
    Serial.println("Trancar...");
    fechadura.biparBuzzer();
    fechadura.trancar();
    fechadura.mudarStatusDeAuth();
    while (digitalRead(botao) == HIGH);
    delay(50);
  }
}

// Função de configuração do Arduino e inicialização dos pinos
void setup() {
  Serial.begin(9600);
  fechadura.begin();
  pinMode(botao, INPUT);
}
// Função de loop principal do Arduino
void loop() {
  verificarTeclado();
  verificarBotao();
}

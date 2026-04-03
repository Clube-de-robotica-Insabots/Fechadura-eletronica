#include <Arduino.h>
#include <Keypad.h>
#include <Bounce2.h>
#include "Fechadura.h"

//Definição dos pinos
int pin1 = 3;
int pin2 = 2;
int botaoPin = 4;
int buzzerPin = 12;

//Instancia da fechadura
Fechadura fechadura(pin1, pin2, buzzerPin);

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

Bounce2::Button button;

// Funções auxiliares
void adicionarDigitoASenha(char key, String *p){
  *p += key;
  Serial.println(*p);
}
void limparSenhaDigitada(String *p){
  *p = "";
  Serial.println("Senha limpa.");
}
void verificarSenha(String *p){  
  if (fechadura.autenticar(*p)){
    Serial.println("Senha correta! Destrancando...");
    fechadura.mudarStatusDeAuth();
    fechadura.destrancar();
  }
  else {
    Serial.println("Senha incorreta! Tente novamente.");
    fechadura.biparBuzzer();
  }
}
void verificarTeclado(){
  // Verificar a senha para destrancar pelo lado de fora
  static String senhaDigitada;
  char key = keypad.getKey();
  if (key && fechadura.statusDaAutenticacao() == false) {
    // Verificar se a tecla pressionada é '#' para validar a senha ou '*' para limpar a senha digitada
    switch (key){
      case '#':
        verificarSenha(&senhaDigitada);
        limparSenhaDigitada(&senhaDigitada);
        break;

      case '*':
        limparSenhaDigitada(&senhaDigitada);
        break;

      default:
        adicionarDigitoASenha(key, &senhaDigitada);
        fechadura.biparBuzzer();
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

// Função de configuração do Arduino e inicialização dos pinos
void setup() {
  Serial.begin(9600);
  fechadura.begin();
  button.attach(botaoPin, INPUT);
  button.interval(25);
  button.setPressedState(HIGH);
}
// Função de loop principal do Arduino
void loop() {
  verificarTeclado();
  if (button.pressed()){
    if (fechadura.statusDaAutenticacao()){
      Serial.println("Botão pressionado! Trancando...");
      fechadura.trancar();
    }
    else {
      fechadura.destrancar();
    }
    fechadura.mudarStatusDeAuth();
  }
}

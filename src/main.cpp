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

// Variável para armazenar a senha digitada
String senhaDigitada;

// Funções auxiliares
void adicionarDigitoASenha(char key){
  senhaDigitada += key;
  Serial.println(senhaDigitada);
}
void limparSenhaDigitada(){
  senhaDigitada = "";
  Serial.println("Senha limpa.");
}
void verificarSenha(){
  if (fechadura.autenticar(senhaDigitada)) {
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
  char key = keypad.getKey();
  if (key && fechadura.statusDaAutenticacao() == false) {
    // Verificar se a tecla pressionada é '#' para validar a senha ou '*' para limpar a senha digitada
    switch (key){
      case '#':
        verificarSenha();
        limparSenhaDigitada();
        break;

      case '*':
        limparSenhaDigitada();
        break;

      default:
        adicionarDigitoASenha(key);
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
bool verificarSeOBotaoEstaPressionado(){
  int buttonState = digitalRead(botao);
  if (buttonState == HIGH) {
    return true;
  }
  else {
    return false;
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
  if (verificarSeOBotaoEstaPressionado()){
    if (fechadura.statusDaAutenticacao() == true){
      Serial.println("Botão pressionado! Trancando...");
      fechadura.trancar();
      fechadura.mudarStatusDeAuth();
    }
    else {
      fechadura.destrancar();
      fechadura.mudarStatusDeAuth();
    }
  }
}

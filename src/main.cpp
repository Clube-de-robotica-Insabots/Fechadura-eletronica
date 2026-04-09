#include <Arduino.h>
#include <Keypad.h>
#include <Bounce2.h>
#include "Fechadura.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
//Definição dos pinos
int pin3 = 3;
int pin4 = 2;
int botaoPin = 12;
int buzzerPin = 7;
//Instancias de objetos
Fechadura fechadura(pin3, pin4, buzzerPin);
Bounce2::Button button;
BlynkTimer timer;

//Configuração do teclado matricial
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {4, 6, 9, 10};
byte colPins[COLS] = {11, 5, 8};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Funções de wifi e config para app
void atualizarStatusApp() {
  if (fechadura.statusDaAutenticacao()) {
    Blynk.virtualWrite(V1, "🔓 Aberta");
  } else {
    Blynk.virtualWrite(V1, "🔒 Fechada");
  }
}
BLYNK_WRITE(V0) {
  int buttonState = param.asInt();
  if (buttonState == 1 && fechadura.statusDaAutenticacao() == true) {
    Serial.println("Botão do app pressionado! Trancando...");
    fechadura.trancar();
    fechadura.mudarStatusDeAuth(false);
    
    }
  else if (buttonState == 0 && fechadura.statusDaAutenticacao() == false) {
    Serial.println("Botão do app pressionado! Destrancando...");
    fechadura.destrancar();
    fechadura.mudarStatusDeAuth(true);
  }
  atualizarStatusApp();
}
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
    fechadura.mudarStatusDeAuth(true);
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
  if (!key) return;
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
      fechadura.trancar();
      fechadura.mudarStatusDeAuth(false);
    }
  }
}
// Função de configuração do Arduino e inicialização dos pinos
void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, SSID_WIFI, PASSWORD_WIFI);
  fechadura.begin();  Blynk.begin(BLYNK_AUTH_TOKEN, SSID_WIFI, PASSWORD_WIFI);
  button.attach(botaoPin, INPUT_PULLUP);
  button.interval(100);
  button.setPressedState(LOW);
  timer.setInterval(2000L, atualizarStatusApp);
  Serial.println("Fechadura pronta!");
}
// Função de loop principal do Arduino
void loop() {
  Blynk.run();
  timer.run();
  verificarTeclado(); 
  button.update();
  if (button.pressed()) {
    if (fechadura.statusDaAutenticacao()){
      Serial.println("Botão pressionado! Trancando...");
      fechadura.trancar();
      fechadura.mudarStatusDeAuth(false);
    }
    else {
      Serial.println("Botão pressionado! Destrancando...");
      fechadura.destrancar();
      fechadura.mudarStatusDeAuth(true);
    }
  }
}

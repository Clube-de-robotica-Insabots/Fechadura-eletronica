#include <Arduino.h>
#include <Keypad.h>
#include <Bounce2.h>
#include "Fechadura.h"
#include <WiFi.h>
#include <PubSubClient.h>
//Definição dos pinos
int pin3 = 3;
int pin4 = 2;
int botaoPin = 12;
int buzzerPin = 7;
//Instancias de objetos
Fechadura fechadura(pin3, pin4, buzzerPin);
Bounce2::Button button;
WiFiClient espClient;
PubSubClient mqtt(espClient);

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
void publicarStatus() {
  if (fechadura.statusDaAutenticacao()) {
    mqtt.publish(TOPIC_STATUS, "ABERTA");
  } else {
    mqtt.publish(TOPIC_STATUS, "FECHADA");
  }
}
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.println("Comando recebido: " + mensagem);

  if (mensagem == "DESTRANCAR" && !fechadura.statusDaAutenticacao()) {
    Serial.println("App: Destrancando...");
    fechadura.destrancar();
    fechadura.mudarStatusDeAuth(true);
    publicarStatus();
  }
  else if (mensagem == "TRANCAR" && fechadura.statusDaAutenticacao()) {
    Serial.println("App: Trancando...");
    fechadura.trancar();
    fechadura.mudarStatusDeAuth(false);
    publicarStatus();
  }
}

void reconectarMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (mqtt.connect(MQTT_CLIENT_ID)) {
      Serial.println(" Conectado!");
      mqtt.subscribe(TOPIC_COMANDO);
      publicarStatus();
    } else {
      Serial.print(" Falhou, rc=");
      Serial.println(mqtt.state());
      delay(3000);
    }
  }
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
    publicarStatus();
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
      publicarStatus();
    }
  }
}
// Função de configuração do Arduino e inicialização dos pinos
void setup() {
  Serial.begin(115200);
  fechadura.begin();

  WiFi.begin(SSID_WIFI, PASSWORD_WIFI);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado! IP: " + WiFi.localIP().toString());

  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(callbackMQTT);

  button.attach(botaoPin, INPUT_PULLUP);
  button.interval(100);
  button.setPressedState(LOW);

  Serial.println("Sistema pronto!");
}
// Função de loop principal do Arduino
void loop() {
  if (!mqtt.connected()) reconectarMQTT();
  mqtt.loop();

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
    publicarStatus();
  }
}

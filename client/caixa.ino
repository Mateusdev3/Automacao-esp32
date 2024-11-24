#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <Ultrasonic.h>
#define TRIGGER_PIN 12
#define ECHO_PIN 14

const char* ssid = "Starlink2.4";
const char* password = "messias314";
const char* servidorIP = "http://192.168.1.44";

Ultrasonic ultra(TRIGGER_PIN, ECHO_PIN);
WebServer server(80);
float cm;
const int led = 2;
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
    digitalWrite(led, HIGH);
  }
  Serial.println("Conectado!");
  Serial.println("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("   ");
  
}
void loop() {
  Controle();
  lerDistancia();
 
}
void lerDistancia() {
  long microsec = ultra.timing();
  cm = ultra.convert(microsec, Ultrasonic::CM);
  Serial.println(cm);
  delay(1000);
}
void Controle(){
    if (cm > 10) {
    AcionarBomba();
    Serial.println("Acionando bomba ");
  } else {
    Serial.println("Desligando bomba");
    DesligarBomba();
  }
}
void AcionarBomba() {
  HTTPClient http;
  String url = String(servidorIP) + "/ligar";
  http.begin(url);
  int httpcode = http.GET();
  if (httpcode > 0) {
    Serial.printf("Requisição GET bem sucedida ", httpcode);
  } else {
    Serial.printf("Erro ao executar requisição GET", httpcode);
  }
}

void DesligarBomba() {
  HTTPClient http;
  String url = String(servidorIP) + "/desligar";
  http.begin(url);
  int httpcode = http.GET();
  if (httpcode > 0) {
    Serial.printf("Requisição GET bem sucedida ", httpcode);
  } else {
    Serial.printf("Erro ao executar requisição GET ", httpcode);
  }
}
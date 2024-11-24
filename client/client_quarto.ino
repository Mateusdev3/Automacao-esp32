#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>

const char *ssid = "Starlink2.4";
const char *senha = "messias314";
const int led = 2;
const int rele1 = 4;
const int rele2 = 6;

WebServer server(80);

void setup()
{
    // comunicaçao serial e modos de pinos
    Serial.begin(115200);
    
    pinMode(led, OUTPUT);
    pinMode(rele1, OUTPUT);
    pinMode(rele2, OUTPUT);
    // Inicializaçao de pinos
    digitalWrite(led, LOW);
    digitalWrite(rele1, HIGH);
    digitalWrite(rele2, HIGH);
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Conectando...");
    }
    Serial.println("Conectado IP: ");
    Serial.print(WiFi.localIP());
        //rotas
        server.on("/", handleRoot);
    server.on("ligar", handleOn);
    server.on("desligar", handleOff);
    Serial.begin();
    Serial.println("Servidor online");
}
void loop()
{
    server.handleClient();
}
void handleRoot()
{
    server.send(200, "text/plain", "Servidor luz do quarto");
}
void handleOn()
{
    digitalWrite(rele1, LOW);
    server.send(200, "text/plain", "Rele Ligado");
}
void handleOff()
{
    digitalWrite(rele1, HIGH);
    server.send(200, "text/plain", "Rele desligado");
}
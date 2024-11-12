#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "NOME_REDE";        // Nome da rede Wi-Fi
const char* password = "SENHA_REDE";   // Senha da rede Wi-Fi

WebServer server(80);                  // Inicializa o servidor na porta 80
const int relePin = 4;                 // Pino conectado ao relé

void setup() {
  Serial.begin(115200);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, HIGH);  // Relé inicialmente desligado (HIGH para relés com lógica invertida)

  // Conectando ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configurando rotas do servidor
  server.on("/", handleRoot);  // Página inicial
  server.on("/ligar", handleLigar);   // Rota para ligar o relé
  server.on("/desligar", handleDesligar); // Rota para desligar o relé

  server.begin();  // Inicia o servidor
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();  // Mantém o servidor respondendo às requisições
}

// Função para a página inicial
void handleRoot() {
  server.send(200, "text/plain", "Servidor Web ESP32 - Controle de Relé");
}

// Função para ligar o relé
void handleLigar() {
  digitalWrite(relePin, LOW);  // Liga o relé
  server.send(200, "text/plain", "Relé Ligado");
}

// Função para desligar o relé
void handleDesligar() {
  digitalWrite(relePin, HIGH);  // Desliga o relé
  server.send(200, "text/plain", "Relé Desligado");
}
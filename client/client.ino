#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHT11_PIN 4




const char* ssid = "Starlink2.4";
const char* password = "messias314";
const char* servidorIP = "http://192.168.1.44";

DFRobot_DHT11 DHT;
WebServer server(80);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pino do ESP32 conectado ao relé
const int relePin = 13;
const int relePin2 = 12;
const int relePin3 = 14;
const int relePin4 = 27;
const int led = 2;

int temp = 0;
int humi = 0;


void setup() {
  pinMode(relePin, OUTPUT);
  pinMode(relePin2, OUTPUT);
  pinMode(relePin3, OUTPUT);
  pinMode(relePin4, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(relePin, HIGH);   // invertido (rele aterrado)
  digitalWrite(relePin2, HIGH);  // invertido (rele aterrado)
  digitalWrite(relePin3, HIGH);  // invertido (rele aterrado)
  digitalWrite(relePin4, HIGH);   // invertido (rele aterrado)
  digitalWrite(led, LOW);

  // Wifi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado!");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  digitalWrite(led, HIGH);

  // Rotas HTTP
  server.on("/", handleRoot);
  server.on("/ligar1", handleLigar);
  server.on("/desligar1", handleDesligar);
  server.on("/ligar2", handleLigar2);
  server.on("/desligar2", handleDesligar2);
  server.on("/ligar3", handleLigar3);
  server.on("/desligar3", handleDesligar3);
  server.on("/ligar4", handleLigar4);
  server.on("/desligar4", handleDesligar4);
  server.on("/ligarRele5", handleLigarRelex1);
  server.on("/desligarRele5", handleDesligarRelex1);
  

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor iniciado!");
}

void loop() {
  server.handleClient();
  Temp();
  Oled();
}
void handleRoot() {

  String pagina = "<!DOCTYPE html>";
pagina += "<html lang='pt'>";
pagina += "<head><meta charset='UTF-8'>";
pagina += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
pagina += "<title>Automação 📱</title></head>";
pagina += "<body>";
pagina += "<div class='fundo'></div>";
pagina += "<div class='main'>";
pagina += "<div class='txt1'><h1>Automação</h1>";
pagina += "<div class='graus'><span>🌡️" + String(temp) + "°C </span><span> 💧" + String(humi) + "% </span></div></div>";
pagina += "<div class='button-container'>";
pagina += "<button id='meubt1' class='b b-1' onclick='toggleRele1()'>Luz 1</button>";
pagina += "<button id='meubt2' class='b b-2' onclick='toggleRele2()'>Luz 2</button>";
pagina += "<button id='meubt3' class='b b-3' onclick='toggleRele3()'>Luz 3</button>";
pagina += "<button id='meubt4' class='b b-4' onclick='toggleRele4()'>Ventilador</button>";
pagina += "<button id='meubt5' class='b b-5' onclick='toggleRele5()'>Bomba</button>";
pagina += "<button id='meubt6' class='b b-6' onclick='Camera1()'>Camera</button>";
pagina += "<button id='meubt7' class='b b-7' onclick='toggleRele7()'>TV</button>";
pagina += "<button id='meubt8' class='b b-8' onclick='toggleRele8()'>Microtik</button>";
pagina += "<button id='meubt9' class='b b-9' onclick='toggleRele9()'>Tomada</button>";
pagina += "</div></div>";

pagina += "<script>";
pagina += "let isRele1On = false;";
pagina += "let isRele2On = false;";
pagina += "let isRele3On = false;";
pagina += "let isRele4On = false;";
pagina += "let isRele5On = false;";

pagina += "function toggleRele1() { isRele1On = !isRele1On; const button1 = document.getElementById('meubt1');";
pagina += "if (isRele1On) { button1.classList.add('on'); button1.classList.remove('off'); ligarRele1();";
pagina += "} else { button1.classList.add('off'); button1.classList.remove('on'); desligarRele1(); } }";

pagina += "function toggleRele2() { isRele2On = !isRele2On; const button2 = document.getElementById('meubt2');";
pagina += "if (isRele2On) { button2.classList.add('on'); button2.classList.remove('off'); ligarRele2();";
pagina += "} else { button2.classList.add('off'); button2.classList.remove('on'); desligarRele2(); } }";

pagina += "function toggleRele3() { isRele3On = !isRele3On; const button3 = document.getElementById('meubt3');";
pagina += "if (isRele3On) { button3.classList.add('on'); button3.classList.remove('off'); ligarRele3();";
pagina += "} else { button3.classList.add('off'); button3.classList.remove('on'); desligarRele3(); } }";

pagina += "function toggleRele4() { isRele4On = !isRele4On; const button4 = document.getElementById('meubt4');";
pagina += "if (isRele4On) { button4.classList.add('on'); button4.classList.remove('off'); ligarRele4();";
pagina += "} else { button4.classList.add('off'); button4.classList.remove('on'); desligarRele4(); } }";

pagina += "function toggleRele5() { isRele5On = !isRele5On; const button5 = document.getElementById('meubt5');";
pagina += "if (isRele5On) { button5.classList.add('on'); button5.classList.remove('off'); ligarRele5();";
pagina += "} else { button5.classList.add('off'); button5.classList.remove('on'); desligarRele5(); } }";

pagina += "function ligarRele1() { var rqr = new XMLHttpRequest(); rqr.open('GET', '/ligar1', true); rqr.send(); }";
pagina += "function desligarRele1() { var rqr = new XMLHttpRequest(); rqr.open('GET', '/desligar1', true); rqr.send(); }";
pagina += "function ligarRele2() { var rqr = new XMLHttpRequest(); rqr.open('GET', '/ligar2', true); rqr.send(); }";
pagina += "function desligarRele2() { var rqr = new XMLHttpRequest(); rqr.open('GET', '/desligar2', true); rqr.send(); }";
pagina += "function ligarRele5() { var rqr = new XMLHttpRequest(); rqr.open('GET', '/ligarRele5', true); rqr.send(); }";
pagina += "function desligarRele5() { var rqr = new XMLHttpRequest(); rqr.open('GET', '/desligarRele5', true); rqr.send(); }";
pagina += "function Camera1() { window.open('http://192.168.1.161', '_blank'); }";
pagina += "</script>";

pagina += "<style>";
pagina += "body { text-align: center; }";
pagina += ".graus span { font-family: Arial, Helvetica, sans-serif; font-size: 40px; text-shadow: 6px 6px 9px black; margin-left: 30px; margin-right: 30px; }";
pagina += ".fundo { background-image: url(https://raw.githubusercontent.com/Theus00637/imagens/refs/heads/main/fundo%20site.png);";
pagina += "background-size: cover; background-repeat: no-repeat; height: 100vh; width: 100vw; position: fixed; top: 0; left: 0;";
pagina += "z-index: -1; opacity: 0.855; background-color: rgba(0, 0, 0, 0.507); }";
pagina += ".main { display: flex; flex-direction: column; justify-content: center; align-items: center; background-color: rgba(0, 0, 0, 0.507); }";
pagina += ".txt1 { color: rgb(245, 237, 237); text-align: center; font-size: 30px; text-shadow: 2px 2px 10px rgba(0, 0, 0, 0.7); }";
pagina += ".button-container { display: flex; justify-content: center; align-items: center; flex-wrap: wrap; }";
pagina += ".b { padding: 60px 90px; min-width: 150px; max-width: 150px; max-height: 100px; background-color: rgb(239, 247, 255); opacity: 0.9;";
pagina += "border-radius: 30px; color: rgb(0, 0, 0); cursor: pointer; font-size: 18px; margin: 10px; text-align: center;";
pagina += "display: flex; justify-content: center; align-items: center; }";
pagina += ".b-1 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/lampadag4.png);";
pagina += "background-position: center; background-size: cover; }";
pagina += ".b-2 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/lampadag4.png); background-position: center; background-size: cover; }";
pagina += ".b-3 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/lampadag4.png); background-position: center; background-size: cover; }";
pagina += ".b-4 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/ventilador.png); background-position: center; background-size: cover; }";
pagina += ".b-5 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/bombag1.png); background-position: center; background-size: cover; }";
pagina += ".b-6 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/camera.png); background-position: center; background-size: cover; }";
pagina += ".b-7 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/tv.png); background-position: center; background-size: cover; }";
pagina += ".b-8 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/microtik.png); background-position: center; background-size: cover; }";
pagina += ".b-9 { background-image: url(https://raw.githubusercontent.com/Theus00637/automa-ao/refs/heads/main/tomada.png); background-position: center; background-size: cover; }";
pagina += ".on { border-color: rgb(0, 255, 0); }";
pagina += "</style></body></html>";


  server.send(200, "text/html", pagina);
}
// Funções
void Temp() {
  DHT.read(DHT11_PIN);
  int t = DHT.temperature;
  int h = DHT.humidity;
  temp = t;
  humi = h;
  delay(1000);
}  
void Oled(){
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Falha ao alocar"));
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(2);  
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);
  display.println("Tempertura");
  display.setCursor(0, 30);
  display.print("C ");
  display.print(temp);
  display.setCursor(60, 30);
  display.print("H ");
  display.print(humi);
  display.display();
}



void handleLigar() {
  digitalWrite(relePin, LOW);
  server.send(200, "text/plain", "Relé ligado");
  Serial.println("Ligando rele 1");
}

void handleDesligar() {
  digitalWrite(relePin, HIGH);
  server.send(200, "text/plain", "Relé desligado");
  Serial.println("desligando rele1");
}

void handleLigar2() {
  digitalWrite(relePin2, LOW);
  server.send(200, "text/plain", "Relé 2 ligado");
}

void handleDesligar2() {
  digitalWrite(relePin2, HIGH);
  server.send(200, "text/plain", "Relé 2 desligado");
}
void handleLigar3(){
  digitalWrite(relePin3, LOW);
  server.send(200, "text/plain", "Relé 3 ligado");

}
void handleDesligar3(){
  digitalWrite(relePin3, HIGH);
  server.send(200, "text/plain", "Rele 3 desligado");
}
void handleLigar4(){
  digitalWrite(relePin4, LOW);
  server.send(200, "text/plain", "Rele 4 Ligado");
}
void handleDesligar4(){
    digitalWrite(relePin4, HIGH);
  server.send(200, "text/plain", "Rele 4 desligado");


}
void handleLigarRelex1() {
  HTTPClient http;
  String url = String(servidorIP) + "/ligar";
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("Requisição GET bem-sucedida, código de status: %d\n", httpCode);
     server.send(200, "text/plain", "Rele bomba ligado");
  } else {
    Serial.printf("Erro na requisição GET: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
void handleDesligarRelex1() {
  HTTPClient http;
  String url = String(servidorIP) + "/desligar";
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("Requisição GET bem-sucedida, código de status: %d\n", httpCode);
     server.send(200, "text/plain", "Rele bomba desligado");
  } else {
    Serial.printf("Erro na requisição GET: %s\n", http.errorToString(httpCode).c_str());  
  }
  http.end();
}

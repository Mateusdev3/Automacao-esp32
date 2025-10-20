# Automação com ESP32, Sensor DHT11 e Display OLED

Este projeto implementa uma automação utilizando um ESP32 para ler dados do sensor de temperatura e umidade DHT11, exibindo as informações em um display OLED e enviando os dados para servidores via HTTP.

---

## Descrição

O sistema conecta o ESP32 a uma rede WiFi, lê temperatura e umidade do sensor DHT11, apresenta os dados no display OLED SSD1306 e envia essas informações para dois servidores diferentes por meio de requisições HTTP.

Além disso, o código inclui um bitmap (imagem) armazenado na memória para exibição no OLED.

---

## Componentes Utilizados

- ESP32
- Sensor DHT11 (pino 4)
- Display OLED 128x64 (SSD1306)
- Rede WiFi

---

## Bibliotecas Necessárias

- `HTTPClient.h` — Para requisições HTTP
- `WiFi.h` — Para conexão WiFi
- `WebServer.h` — (se aplicável para servidor local)
- `Arduino.h`
- `DFRobot_DHT11.h` — Para leitura do sensor DHT11
- `Wire.h` — Para comunicação I2C com OLED
- `Adafruit_GFX.h` — Biblioteca gráfica para OLED
- `Adafruit_SSD1306.h` — Controle do display OLED SSD1306

---

## Configuração

No código, configure as seguintes variáveis para a sua rede e servidores:

```cpp
const char* ssid = "Seu_SSID";
const char* password = "Sua_Senha";
const char* servidorIP = "http://IP_do_servidor_1";
const char* servidorIP1 = "http://IP_do_servidor_2";
#define DHT11_PIN 4 // Pino conectado ao DHT11

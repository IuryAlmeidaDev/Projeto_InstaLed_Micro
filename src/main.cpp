#include <WiFi.h>
#include <WebSocketsClient.h>
#include <WiFiClientSecure.h>

#include "FS.h"

const char *ssid = "Scooby-Doo";
const char *password = "10203040";

WebSocketsClient webSocket;

int ledPin = 2; // Defina o pino do LED

void setup()
{
  Serial.begin(115200); // Configura a comunicação serial

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin, OUTPUT);

  // Conectar ao servidor WebSocket
  webSocket.begin("seu_servidor_websocket", 8080, "/"); // Defina o IP do servidor e a porta

  // Configurar callback para quando receber mensagens do WebSocket
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  // Manter a conexão com o WebSocket
  webSocket.loop();
}

// Callback para quando uma mensagem é recebida do WebSocket
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_CONNECTED:
    Serial.println("Conectado ao WebSocket");
    break;
  case WStype_DISCONNECTED:
    Serial.println("Desconectado do WebSocket");
    break;
  case WStype_TEXT:
    String msg = String((char *)payload);
    Serial.println("Mensagem recebida: " + msg);

    if (msg == "ligar_led")
    {
      digitalWrite(ledPin, HIGH); // Ligar o LED
      Serial.println("LED Ligado");
    }
    else if (msg == "desligar_led")
    {
      digitalWrite(ledPin, LOW); // Desligar o LED
      Serial.println("LED Desligado");
    }
    break;
  }
}

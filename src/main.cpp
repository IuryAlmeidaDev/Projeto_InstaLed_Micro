#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Scooby-Doo";
const char *password = "10203040";

const char *apiUrl = "https://projeto-instaled-api-1.onrender.com/led/state";
const int relayPin = 23;

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(apiUrl);

    int httpResponseCode = http.GET();

    if (httpResponseCode == 200)
    {
      String response = http.getString();
      Serial.println("Resposta da API: " + response);

      // Analisa o JSON retornado
      if (response.indexOf("\"state\":true") != -1)
      {
        digitalWrite(relayPin, LOW); // Liga o relé
        Serial.println("Relé ligado");
      }
      else if (response.indexOf("\"state\":false") != -1)
      {
        digitalWrite(relayPin, HIGH); // Desliga o relé
        Serial.println("Relé desligado");
      }
      else
      {
        Serial.println("Resposta JSON inválida!");
      }
    }
    else
    {
      Serial.println("Erro ao acessar a API: " + String(httpResponseCode));
    }

    http.end();
  }
  else
  {
    Serial.println("Wi-Fi desconectado! Tentando reconectar...");
    WiFi.reconnect();
  }

  delay(5000);
}

// tentando subir esse trem

#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WebServer.h>


#include "config.h"

String v = "0.3";

WebServer server(80);

const int led = LED_BUILTIN;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hola desde esp32!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}


void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(nombreESP32)) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "tambien funciona asi...");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server arrancado");
  String url="http://";
  url += WiFi.localIP().toString();
  Serial.println(url);

}

void loop() {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}

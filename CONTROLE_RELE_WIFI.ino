#include <ESP8266WiFi.h>
#ifndef STASSID
#define STASSID "LaPeSi/Lims"
#define STAPSK  "naotemsenha"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
int esquerda = D4; // GPIO
int direita = D5; // GPIO
bool desliga=HIGH;
bool liga=LOW;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
// prepare LED
  pinMode(direita, OUTPUT);
  digitalWrite(direita, desliga);
  pinMode(esquerda, OUTPUT);
  digitalWrite(esquerda, desliga);
  delay(10);
 // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  server.begin();                       // Start the server
  Serial.println(F("Server started"));
  Serial.println(WiFi.localIP());       // Print the IP address
}

void loop() {
// WIFI
  WiFiClient client = server.available();   // Check if a client has connected
  if (!client) {return;}
  Serial.println(F("new client"));
  client.setTimeout(5000); // default is 1000
  String req = client.readStringUntil('\r');    // Read the first line of the request
  Serial.println(F("request: "));
  Serial.println(req);
// Match the request
  int val1,val2;
  if (req.indexOf(F("/gpio/0")) != -1) {
    val1 = 0;
     digitalWrite(esquerda, desliga);
     digitalWrite(direita, desliga);
  } else if (req.indexOf(F("/gpio/1/on")) != -1) {
    val1 = 1;
    val1 = digitalRead(esquerda);
    digitalWrite(esquerda, liga);
    Serial.println(F("LÃMPADA1 LIGADA!"));
  } else if (req.indexOf(F("/gpio/1/off")) != -1) {
    val1 = 1;
    val1 = digitalRead(esquerda);
    digitalWrite(esquerda, desliga);
    Serial.println(F("LÃMPADA1 DESLIGADA!"));
  } else if (req.indexOf(F("/gpio/2/on")) != -1) {
    val2 = 1;
    val2 = digitalRead(direita);;
    digitalWrite(direita, liga);
    Serial.println(F("LÃMPADA2 LIGADA!"));
  } else if (req.indexOf(F("/gpio/2/off")) != -1) {
    val2 = 1;
    val2 = digitalRead(direita);
    digitalWrite(direita, desliga);
    Serial.println(F("LÃMPADA2 DESLIGADA!"));
  } else {
    Serial.println(F("Pedido inválido!"));
    val1 = digitalRead(LED_BUILTIN);
  }
while (client.available()) {client.read();}              // Enquanto houver solicitação, ler/ignorar o restante da solicitação

// Envia a resposta para o cliente que está OK para vários "client.print/write", porque o algoritmo os agrupará em um único pacote
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nO rele 1 esta: "));
  client.print((val1) ? F("LIGADO") : F("DESLIGADO"));
  client.print(F("<br><br>O rele 2 esta: "));
  client.print((val2) ? F("LIGADO") : F("DESLIGADO"));
  client.print(F("<br><br>Clique <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>AQUI</a> para ligar o led1 ou <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/2'>AQUI</a> para desligar o led1 ou <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/3'>AQUI</a> para ligar o led2 ou <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/4'>AQUI</a> para desligar o led2.</html>"));
  
// O cliente será 'liberado' e desconectado quando a função retornar e o objeto 'cliente' for destruído, garantindo que os dados gravados sejam recebidos pelo outro lado
  Serial.println(F("Desconectando do cliente!!!"));
}

#include <ESP8266WiFi.h>
#ifndef STASSID

// rede da minha casa e meu celular
//#define STASSID "STWA_JR"
//#define STAPSK  "774881jr"

// rede LIMS
#define STASSID "LaPeSi/Lims"
#define STAPSK  "naotemsenha"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
int direita = D2;           // GPIO
int direita_inverte = D3;   // GPIO
int esquerda = D4;          // GPIO
int esquerda_inverte = D5;  // GPIO


// Criação do servidor
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
// Preparação dos motores
  pinMode(direita, OUTPUT);
  pinMode(direita_inverte, OUTPUT);
  pinMode(esquerda, OUTPUT);
  pinMode(esquerda_inverte, OUTPUT);
  delay(10);
// Conexão com WIFI
  Serial.println();
  Serial.println();
  Serial.print(F("Conectando a "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  server.begin();                               //Inicia o servidor
  Serial.println(F("Server started"));
  Serial.println(WiFi.localIP());               //Imprime o IP da conexão
}

void loop() {
// WIFI
  WiFiClient client = server.available();       //Checa se há conexão
  if (!client) {return;}
  Serial.println(F("new client"));
  client.setTimeout(5000);                      //O padrão é 1000
  String req = client.readStringUntil('\r');    //Lê a primeira linha da solicitação
  Serial.println(F("request: "));
  Serial.println(req);
  int val;
  if (req.indexOf(F("/gpio/F")) != -1) {
    digitalWrite(direita, HIGH);
    digitalWrite(esquerda, HIGH);
    digitalWrite(direita_inverte, LOW);
    digitalWrite(esquerda_inverte, LOW);
  } else if (req.indexOf(F("/gpio/T")) != -1) {
    digitalWrite(direita, LOW);
    digitalWrite(esquerda, LOW);
    digitalWrite(direita_inverte, HIGH);
    digitalWrite(esquerda_inverte, HIGH); 
  } else if (req.indexOf(F("/gpio/E")) != -1) {
    digitalWrite(direita, HIGH);
    digitalWrite(esquerda, LOW);
    digitalWrite(direita_inverte, LOW);
    digitalWrite(esquerda_inverte, HIGH);
  } else if (req.indexOf(F("/gpio/D")) != -1) {
    digitalWrite(direita, LOW);
    digitalWrite(esquerda, HIGH);
    digitalWrite(direita_inverte, HIGH);
    digitalWrite(esquerda_inverte, LOW);

    /*
  } else if (req.indexOf(F("/gpio/A")) != -1) {
    curva = 1;
    while (curva == 1){
      redutor = redutor +1;
      if (redutor%10==0){
        Serial.println("redutor");
        digitalWrite(direita, HIGH);
        digitalWrite(direita_inverte, LOW);
      }
      digitalWrite(esquerda, HIGH);
      digitalWrite(esquerda_inverte, LOW);
      Serial.println(redutor);
      if (redutor == 50){
        redutor =0;
      }
    }
    curva = 0;
  } else if (req.indexOf(F("/gpio/FE")) != -1) {
    if (redutor%2==0){
      digitalWrite(esquerda, LOW);
      redutor = redutor +1;
    }
    digitalWrite(direita, LOW);
    digitalWrite(direita_inverte, HIGH);
    digitalWrite(esquerda_inverte, HIGH);
  } else if (req.indexOf(F("/gpio/TD")) != -1) {
    if (redutor%2==0){
      digitalWrite(direita_inverte, LOW);
      redutor = redutor +1;
    }
    digitalWrite(esquerda_inverte, LOW);
    digitalWrite(direita, HIGH);
    digitalWrite(esquerda, HIGH);
  } else if (req.indexOf(F("/gpio/TE")) != -1) {
    if (redutor%2==0){
      digitalWrite(esquerda_inverte, LOW);
      redutor = redutor +1;
    }
    digitalWrite(direita_inverte, LOW);
    digitalWrite(direita, HIGH);
    digitalWrite(esquerda, HIGH);
    */
  } else if (req.indexOf(F("/gpio/0")) != -1) {
    digitalWrite(direita, LOW);
    digitalWrite(esquerda, LOW);
    digitalWrite(direita_inverte, LOW);
    digitalWrite(esquerda_inverte, LOW);
  }

while (client.available()) {client.read();}              // Enquanto houver solicitação, ler/ignorar o restante da solicitação
// Envia a resposta para o cliente que está OK para vários "client.print/write", porque o algoritmo os agrupará em um único pacote
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\rFUNCIONANDO!"));
}

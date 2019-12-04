// WIFI
#include <ESP8266WiFi.h>
#ifndef STASSID
#define STASSID "STWA_JR"
#define STAPSK  "774881jr"
#define ler A0
#endif
int iniciacao=0;
float diferenca=1;

// GPS
#include <SoftwareSerial.h>
#include <TinyGPS.h>

// CONSTANTES WIFI
const char* ssid = STASSID;
const char* password = STAPSK;
WiFiServer server(80);

//CONSTANTES GPS
SoftwareSerial serial1(D6, D7); // RX, TX
TinyGPS gps1;
float lati=0,lati_anterior=1;
float longi=0,longi_anterior=1;

void setup() {
   serial1.begin(9600);
   Serial.begin(115200);
// PROGRAMA WIFI
  Serial.print(F("Conectando à rede "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);                        // Tipo de conexão
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {     // Equanto não conectado
    delay(500);
    Serial.print(F("."));                     // Imprime um "."
  }
  Serial.println();
  Serial.println(F("WiFi conectado"));        // Informa que foi conectado
  server.begin();                             // Inicia o servidor
  Serial.println(F("Servidor iniciado"));     // Informa que o servidor foi iniciado
  Serial.println(WiFi.localIP());             // Imprime o IP
}
void loop() {
// PROGRAMA GPS
  bool recebido = false;
  while (serial1.available()) {
     char cIn = serial1.read();
     recebido = gps1.encode(cIn);
  }
  if (recebido) {
     Serial.println("----------------------------------------");
//Latitude e Longitude
     long latitude, longitude;
     gps1.get_position(&latitude, &longitude);
     lati = latitude;
     longi = longitude;
     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        diferenca = lati-lati_anterior;
        if(abs(diferenca)>0.00001){
          Serial.print("Latitude: ");
          Serial.println(float(latitude)/1000000,6);
          lati_anterior=lati;
        }
     }
     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        diferenca=longi-longi_anterior;
        if(abs(diferenca)>0.00001){
          Serial.print("Longitude: ");
          Serial.println(float(longitude)/1000000, 6);
        }
     }
  }

// PROGRAMA WIFI
  WiFiClient client = server.available();     // Checa se um cliente foi conectado
  if (!client) {return;}                      // Se não tiver conexão, retorna
  Serial.println(F("nova ação"));
  client.setTimeout(5000);                    // O padrão é 1000
  String req = client.readStringUntil('\r');  // Lê a primeira linha do pedido
  Serial.println(F("Pedido: "));
  Serial.println(req);
  while (client.available()) {client.read();} // Enquanto houver solicitação, ler/ignorar o restante da solicitação
// Envia a resposta para o cliente que está OK por vários "client.print/write", porque o algoritmo os agrupará em um único pacote
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\rLatitude: "));
  client.print(float(lati/1000000),6);
  client.print(F("Longitude: "));
  client.print(float(longi/1000000),6);
}

#include <ESP8266WiFi.h>
#ifndef STASSID

// rede do meu celular
#define STASSID "STWA_JR"                     // Nome da rede a ser usada
#define STAPSK  "774881jr"                    // Senha da rede a ser usada

// rede LIMS
//#define STASSID "LaPeSi/Lims"
//#define STAPSK  "naotemsenha"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
int direita = D2;                             // CONECTAR O MOTOR DIREITO AO PINO D2
int direita_inverte = D3;                     // CONECTAR O MOTOR DIREITO INVERTIDO AO PINO D3
int esquerda = D4;                            // CONECTAR O MOTOR ESQUERDO AO PINO D4
int esquerda_inverte = D5;                    // CONECTAR O MOTOR ESQUERDO INVERTIDO AO PINO D5

WiFiServer server(80);                        // Criação do servidor

void andarparafrente(){                       // Funções dos Movimentos
    digitalWrite(direita, HIGH);
    digitalWrite(esquerda, HIGH);
    digitalWrite(direita_inverte, LOW);
    digitalWrite(esquerda_inverte, LOW);
}
void andarparatras(){
    digitalWrite(direita, LOW);
    digitalWrite(esquerda, LOW);
    digitalWrite(direita_inverte, HIGH);
    digitalWrite(esquerda_inverte, HIGH); 
}
void andarparaesquerda(){
    digitalWrite(direita, HIGH);
    digitalWrite(esquerda, LOW);
    digitalWrite(direita_inverte, LOW);
    digitalWrite(esquerda_inverte, HIGH);
}
void andarparadireita(){
    digitalWrite(direita, LOW);
    digitalWrite(esquerda, HIGH);
    digitalWrite(direita_inverte, HIGH);
    digitalWrite(esquerda_inverte, LOW);
}
void parar(){
    digitalWrite(direita, LOW);
    digitalWrite(esquerda, LOW);
    digitalWrite(direita_inverte, LOW);
    digitalWrite(esquerda_inverte, LOW);
}

void setup() {                                                                      //Configuração do programa
  Serial.begin(115200);                                                             //Velocidade de comunicação do Wemos D1
  pinMode(direita, OUTPUT);                                                         // Preparação dos motores
  pinMode(direita_inverte, OUTPUT);
  pinMode(esquerda, OUTPUT);
  pinMode(esquerda_inverte, OUTPUT);
  delay(10);
  Serial.println();                                                                 // Conexão com o WIFI
  Serial.println();
  Serial.print(F("Conectando a "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(F("."));}
  Serial.println();
  server.begin();                                                                   //Inicia o servidor
  Serial.println(F("Rede conectada e Servidor iniciado!"));
  Serial.print(F("IP do carro: ")); Serial.println(WiFi.localIP());                 //Imprime o IP do carro                                                  
}

void loop() {
  WiFiClient client = server.available();                                           //Checa se há conexão
  if (!client) {return;}
  client.setTimeout(1000);                                                          //Tempo de espera da resposta
  String req = client.readStringUntil('\r');                                        //Lê a ULR do servidor
  Serial.print(F("COMANDO: "));
  Serial.println(req);
  if      (req.indexOf(F("/gpio/F")) != -1) {andarparafrente();}
  else if (req.indexOf(F("/gpio/T")) != -1) {andarparatras();}
  else if (req.indexOf(F("/gpio/E")) != -1) {andarparaesquerda();}
  else if (req.indexOf(F("/gpio/D")) != -1) {andarparadireita();}
  else if (req.indexOf(F("/gpio/0")) != -1) {parar();}
  while (client.available()) {client.read();}              // Enquanto houver solicitação, repete a solicitação
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\rIP:"));
  client.print(WiFi.localIP());
}

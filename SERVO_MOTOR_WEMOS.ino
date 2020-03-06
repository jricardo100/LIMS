#include <ESP8266WiFi.h>
#include <Servo.h>
#ifndef STASSID
#define STASSID "STWA_JR1"                     // Nome da rede a ser usada
#define STAPSK  "774881jr"                    // Senha da rede a ser usada
#endif

Servo myservo;                              // Cria o objeto myservo
static const int servoPin = D6;            // Servo conectado à porta D6

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);                      // Criação do servidor na porta 80
String header;                              // Variável to store the HTTP request

String valueString = String(5);             // Decode HTTP GET value
int pos1 = 0;
int pos2 = 0;

void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin);                 // attaches the servo on the servoPin to the servo object
  Serial.println();                         
  Serial.print(F("Conectando a "));
  Serial.println(ssid);                     // Informa a rede à qual está se conectando
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);               // Inicia a conexão com o WIFI
  while (WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(F("."));}  // Enquanto não conecta, imprime um "." no monitor serial
  Serial.println();
  server.begin();                                                                   //Inicia o servidor
  Serial.println(F("Rede conectada e Servidor iniciado!"));
  Serial.print(F("IP do carro: ")); Serial.println(WiFi.localIP());                 //Imprime o IP do carro                                                  
}

void loop(){
  WiFiClient client = server.available();   //Checa se há comando
  if (client) {                             // Se há um comando
    Serial.println("New Client.");          // Imprime a mensagem no serial monitor
    String currentLine = "";                // Cria uma String para guardar o comando
    while (client.connected()) {            // Repete o comando enquanto ele estiver ativo
      if (client.available()) {             // Se o comando estiver ativo
        char c = client.read();             // Lê o comando
        Serial.write(c);                    // Imprime o comando no serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Conexão: fechada");
            client.println();
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
            // Web Page
            client.println("</head><body)><h1>Controle dos Servomotores com Wemos</h1>");
            client.println("<p>Posicao: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
            client.println("</body></html>");     
            if(header.indexOf("GET /?value=")>=0) {                 //Pega o valor do Slider
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              myservo.write(valueString.toInt());                   //Rotaciona o servo
              Serial.println(valueString); 
            }         
            client.println();                                       // The HTTP response ends with another blank line
            
            break;                                                  // Break out of the while loop
          } else {                                                  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {                                     // if you got anything else but a carriage return character,
          currentLine += c;                                         // add it to the end of the currentLine
        }
      }
    }
    header = "";                                                    // Clear the header variable
    client.stop();                                                  // Finaliza a conexão
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

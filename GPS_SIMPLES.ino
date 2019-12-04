#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial serial1(D6, D7); // RX, TX
TinyGPS gps1;

void setup() {
   serial1.begin(9600);
   Serial.begin(115200);
   Serial.println("Aguardando pelo sinal dos satelites...");
}

void loop() {
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
     
     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Latitude: ");
        Serial.println(float(latitude) / 1000000, 6);
     }
     
     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Longitude: ");
        Serial.println(float(longitude) / 1000000, 6);
     }
//satelites
     unsigned short satelites;
     satelites = gps1.satellites();
     if (satelites != TinyGPS::GPS_INVALID_SATELLITES) {
        Serial.print("Satelites: ");
        Serial.println(satelites);
     }
       delay(5000);
  }

}

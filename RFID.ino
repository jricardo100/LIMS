#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10
#define RST_PIN 9
// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 

// Leds indicadores acesso liberado ou negado
int led_liberado = 5;
int led_negado = 6;

char st[20];

void setup(){
  pinMode(led_liberado, OUTPUT);
  pinMode(led_negado, OUTPUT);
  Serial.begin(9600);
  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}

void loop(){
  // Aguarda a aproximacao do cartao
  if ( ! mfrc522.PICC_IsNewCardPresent()){return;}
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial()){return;}
  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++){
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  // Testa se o cartao1 foi lido
  if (conteudo.substring(1) == "F9 9E 3D D5"){
    // Acende o led verde
    digitalWrite(led_liberado, HIGH);
    Serial.println("Cartao1 - Acesso liberado !");
    Serial.println("PAI");
    Serial.println();
    delay(3000);
    digitalWrite(led_liberado, LOW);
    }
    
  // Testa se o cartao2 foi lido
  if (conteudo.substring(1) == "0D CA 3D D5"){
    // Acende o led verde
    digitalWrite(led_liberado, HIGH);
    Serial.println("Cartao1 - Acesso liberado !");
    Serial.println("MÃE");
    Serial.println();
    delay(3000);
    digitalWrite(led_liberado, LOW);
    }

      if (conteudo.substring(1) == "00 15 7E A6"){
    // Acende o led verde
    digitalWrite(led_liberado, HIGH);
    Serial.println("Cartao1 - Acesso liberado !");
    Serial.println("FILHO");
    Serial.println();
    delay(3000);
    digitalWrite(led_liberado, LOW);
    }

      if (conteudo.substring(1) == "75 71 DA D9"){
    Serial.println("Cartao2 - Acesso negado !!");
    Serial.println("USUÁRIO DESCONHECIDO!!!");
    Serial.println();
    // Pisca o led vermelho
    for (int i= 1; i<5 ; i++)
    {
      digitalWrite(led_negado, HIGH);
      delay(200);
      digitalWrite(led_negado, LOW);
      delay(200);
    }
  }

  delay(1000);
}

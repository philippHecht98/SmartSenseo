#include "arduino_secrets.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define WEIGHT_STEP 2000 / 1024

#define WEIGHT_PIN A0

MFRC522 mfrc522(18, RST_PIN);


char ssid[] = SECRET_SSID;
char password[] = SECRET_PASSWORD;

void init_serial() {
  Serial.begin(9600);
}

void init_wifi() {

}

void init_rfid() {
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  Serial.print(F("Scan PICC to see UID, type, and data blocks"));
}

void setup() {
  // put your setup code here, to run once:
  init_serial();
  while(!Serial); // Wait until connection to host is established
  init_rfid();
  Serial.print(F("finished initialising"));
}

void loop() {
  // put your main code here, to run repeatedly:

  // 1. check if rfid token is recognized gets iterated over again in the next loop call
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // 2. rfid token is recognized

    // 2.1 check if power is on

      // - power is off
      // power on coffee machine

      // - else
      
  /*
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  */
  // Step 1
//Informationsabruf des RFID-Gerätes


}
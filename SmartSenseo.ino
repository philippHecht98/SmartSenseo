#include "arduino_secrets.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define WEIGHT_STEP 5 / 1024

MFRC522 mfrc522(SS_PIN, RST_PIN);


char ssid[] = SECRET_SSID;
char password[] = SECRET_PASSWORD;

void init_weight_sensor() {
  // TODO: check if Analog Pin is right
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void init_wifi() {

}

void init_rfid() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  Serial.print(F("Scan PICC to see UID, type, and data blocks"));
}

void setup() {
  // put your setup code here, to run once:
  init_weight_sensor();
  init_rfid();
}

double fetch_weight() {
  // TODO: check if Analog Pin is right
  int analog_weight = analogRead(A0);
  return analog_weight * WEIGHT_STEP;
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
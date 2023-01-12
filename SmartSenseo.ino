#include "arduino_secrets.h"
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 28
#define SS_PIN 18
#define POWER_BUTTON 22
#define SMALL_COFFEE 23
#define BIG_COFFEE 24

#define small_signature 0x00
#define big_signature 0xFF


MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key; 
MFRC522::StatusCode status;

int block=2;

byte readblockcontent[18];

void simulate_button_pressed(int pin) {
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);  
}

void init_pcb() {
  pinMode(POWER_BUTTON, OUTPUT);
  pinMode(SMALL_COFFEE, OUTPUT);
  pinMode(BIG_COFFEE, OUTPUT);
}

void init_rfid() {
  Serial.begin(9600);
  while (!Serial)
    ;
  SPI.begin();
  mfrc522.PCD_Init();
  // Library Version
  mfrc522.PCD_DumpVersionToSerial();
  Serial.print(F("Scan PICC to see UID, type, and data blocks"));
}

boolean is_card_present() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    Serial.print("no new card presented\n");
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    Serial.print("no picc card serial\n");
    return false;
  }
  return true;
}

boolean is_coffee_small() {
  return readblockcontent[0] == small_signature;
}

// https://lastminuteengineers.com/how-rfid-works-rc522-arduino-tutorial/
int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector

  Serial.print("read block\n");

  //authentication of the desired block for access
  /*
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): \n");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
  */

  //reading a block
  byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
  //status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);

  if (status != MFRC522::STATUS_OK) {
      Serial.print(status);
          Serial.print("MIFARE_read() failed: \n");
          Serial.print(mfrc522.GetStatusCodeName(status));
          return 4;//return "4" as error message
  }
  Serial.println("block was read");
}

void setup() {
  // put your setup code here, to run once:
  init_rfid();

  // schalte Kaffeemaschine an
  simulate_button_pressed(POWER_BUTTON);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;  //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
  }

  //delay(30000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Suche nach neuen Karten
  
  if (is_card_present()) {
    // Karte wurde erkannt
    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));    
    readBlock(block, readblockcontent);
    if(is_coffee_small()) {
      //simulate_button_pressed(SMALL_COFFEE);
      Serial.print("brew smoll coffee\n");
      delay(21 * 1000);
      Serial.print("finished brewing smoll coffee\n");
    } else {
      //simulate_button_pressed(BIG_COFFEE);
      Serial.print("brew bigg coffee\n");
      delay(41 * 1000);
    }
    mfrc522.PCD_StopCrypto1();
  }
  
  //Informationsabruf des RFID-Gerätes

  delay(1000);
}
/*

void loop() {
  //Suche nach neuen Karten
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Informationsabruf des RFID-Gerätes
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

*/
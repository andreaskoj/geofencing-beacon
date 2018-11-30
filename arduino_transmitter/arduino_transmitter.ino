// TRANSMITTER CODE - UNO

// to write structures in eeprom
#include "EEPROMAnything.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// verticle object sizeof = 16 bytes
struct VerticleEEPROM
{
    int verticleNumber;
    char x[7];
    char y[7];
};

RF24 radio(7, 8); // CE, CSN
char text[200];
char text2[200];
int i = 0;
//broadcast address
const byte address[6] = "00001";
const int eepromVerticleSize = 16;
const int deviceID;
//storing qty of verticies 
int qtyVerticles;
VerticleEEPROM v1, v2, v3, v4;

void setup() {
Serial.begin(9600); 
  v1.verticleNumber = 0;
  strcpy(v1.x, "1.2233");
  strcpy(v1.y, "5.1234");
  
  v2.verticleNumber = 1;
  strcpy(v2.x, "6.5556");
  strcpy(v2.y, "9.7785");

  v3.verticleNumber = 2;
  strcpy(v3.x, "7.5369");
  strcpy(v3.y, "4.7107");

  v4.verticleNumber = 3;
  strcpy(v4.x, "3.5559");
  strcpy(v4.y, "2.0257");
 
  // check qty of verticies (stored in address 0)
  EEPROM_readAnything(0, qtyVerticles); 

  VerticleEEPROM verticlesArray[qtyVerticles];
  
    for(int i = 0; i<qtyVerticles; i++ ){
      EEPROM_readAnything(2+i*eepromVerticleSize, verticlesArray[i]);
    }

    Serial.println(verticlesArray[0].verticleNumber);
    Serial.println(verticlesArray[0].x);
    Serial.println(verticlesArray[0].y);


 //read from eeprom and reconstruct objects of v.
  
//    EEPROM_writeAnything(2 , v1);
//    EEPROM_writeAnything(2+eepromVerticleSize, v2);
//    EEPROM_writeAnything(2+eepromVerticleSize*2, v3);
//    EEPROM_writeAnything(2+eepromVerticleSize*3, v4);

//  Serial.print("Size of struct ");
//  Serial.println(sizeof(VerticleEEPROM));
//
//  EEPROM_readAnything(0, qtyVerticles);
//  Serial.println(qtyVerticles);
//  
//  EEPROM_readAnything(2, v1);
//  Serial.println(v1.verticleNumber);
//  Serial.println(v1.x);
//  Serial.println(v1.y);
//  EEPROM_readAnything(2+eepromVerticleSize, v2);
//  
//  Serial.println(v2.verticleNumber);
//  Serial.println(v2.x);
//  Serial.println(v2.y);
//  
//  EEPROM_readAnything(2+eepromVerticleSize*2, v3);
//  Serial.println(v3.verticleNumber);
//  Serial.println(v3.x);
//  Serial.println(v3.y);
//
//  EEPROM_readAnything(2+eepromVerticleSize*3, v3);
//  Serial.println(v4.verticleNumber);
//  Serial.println(v4.x);
//  Serial.println(v4.y);

//    EEPROM_readAnything(1+1*eepromVerticleSize, v4);
//  EEPROM_readAnything(1+eepromVerticleSize*3, v4);
//  Serial.println(v4.verticleNumber);


  // starting transimission on nRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  //char text[] = "9,6,1,1.09038,1.02346";
  //radio.write(&text, sizeof(text));
  char text2[] = "9,6,2,7.09038,5.02346";
  radio.write(&text2, sizeof(text2));
  delay(1000);
  
 

}

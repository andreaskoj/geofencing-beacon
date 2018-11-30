// TRANSMITTER CODE - UNO

// Mode types; create=0, broadcast=1, update=2, delete=3

//TODO
// - limit max verticles to store 

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

VerticleEEPROM * verticlesArray;

//broadcast address
const byte address[6] = "00001";
const int eepromVerticleSize = 16;
int deviceID = 0;
//storing qty of verticies 
int qtyVerticles;
int counter = 0;
char charBuffer[32];
void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  // check qty of verticies (stored in address 0)
  EEPROM_readAnything(0, qtyVerticles); 

  verticlesArray = (VerticleEEPROM *) malloc( qtyVerticles * sizeof(VerticleEEPROM) ); 
 
  //get data from EEPROM  
  for(int i = 0; i<qtyVerticles; i++ ){
    EEPROM_readAnything(sizeof(qtyVerticles)+i*eepromVerticleSize, verticlesArray[i]);
  }

  // starting transimission on nRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
}
void loop() {
  int msgNumber = counter % qtyVerticles ;

  //creating a msg
  String msg = String(deviceID)+","+String(1)+","+String(qtyVerticles)+","+String(verticlesArray[msgNumber].verticleNumber)+
  ","+String(verticlesArray[msgNumber].x)+","+String(verticlesArray[msgNumber].y);       
  
  //converting String to char Array
  msg.toCharArray(charBuffer, 32); 
     
  radio.write(&charBuffer, sizeof(charBuffer));
  //counter of the loop
  counter++;
  delay(1000);
}

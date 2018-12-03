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
const byte writeAddress[6] = "00001";
const byte readAddress[6] = "00002";
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

  // starting sending messages on nRF24L01

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(writeAddress);
  radio.openReadingPipe(0, readAddress);
  radio.setRetries(3,5);
  radio.startListening();
}



void loop() {

   receiveData();
    delay(250);
    sendData();
    delay(250);
  
//  int msgNumber = counter % qtyVerticles ;
//
//  String msg = String(deviceID)+","+String(1)+","+String(qtyVerticles)+","+String(verticlesArray[msgNumber].verticleNumber)+
//  ","+String(verticlesArray[msgNumber].x)+","+String(verticlesArray[msgNumber].y);       
//
//  msg.toCharArray(charBuffer, 32); 
//     
//  radio.write(&charBuffer, sizeof(charBuffer));
//
//  counter++;

}

void sendData() {
        char test[20] = "Get from uno";
        radio.stopListening();
            radio.write( &test , sizeof(test) );
            //Serial.println(rslt);
        radio.startListening();

    }
void receiveData() {

      if (radio.available()) {
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.println(text);
      }
}

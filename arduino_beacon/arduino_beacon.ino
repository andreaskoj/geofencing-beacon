// BEACON CODE - UNO

// Mode types; delete=0, add=1

// to write structures in eeprom
#include "EEPROMAnything.h"
#include "beaconLibrary.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// structure def


RF24 radio(7, 8); // CE, CSN

//constants
const int maxVertices = 10;
const int beaconID = 1;
const byte writeAddress[6] = "00002";
const byte readAddress[6] = "00001";

//declaring Array to store vertices in EEPROM
VerticeInEEPROM VerticesArrayEEPROM[maxVertices];

//received message struct
crudMsg crudMsg;

//variables
int qtyVertices;
unsigned int currentMsgToSend;
boolean performingAnalyze = false;

//counters
unsigned int sendDataCounter = 0;

// method prototypes
void broadcastVertices();
boolean analyzeReceivedData();

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  // EEPROM DATA STRUCTURE
  // 1 * qtyVertices[2 bytes]
  // 10 * VerticeInEEPROM[10 bytes]

  // check qty of verticies from EEPROM
  EEPROM_readAnything(0, qtyVertices);

  //get data from EEPROM
  for (int i = 0; i < qtyVertices; i++ ) {
    EEPROM_readAnything(sizeof(qtyVertices) + i * sizeof(VerticeInEEPROM), VerticesArrayEEPROM[i]);
  }

  // starting sending messages on nRF24L01
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(writeAddress);
  radio.openReadingPipe(0, readAddress);
  radio.startListening();
  //fillEEPROM();
}

void loop() {
  sendData();
  delay(250);
  receiveData();
  delay(250);
}

void sendData() {
  radio.stopListening();
  broadcastVertices();
  radio.startListening();
}

void receiveData() {
  if (radio.available()) {

    if (performingAnalyze == false) {
      radio.read(&crudMsg, sizeof(crudMsg));
      //Serial.println(crudMsg.crudId);
      if (crudMsg.beaconId == beaconID) {
        performingAnalyze = true;
        performingAnalyze = analyzeReceivedData();
      }
    }
  }
}

void broadcastVertices() {

  sendDataCounter++;

  // get number of verticle to send
  currentMsgToSend = sendDataCounter % qtyVertices ;

  radio.write(&msgToTrans(VerticesArrayEEPROM[currentMsgToSend], beaconID, qtyVertices),
              sizeof(msgToTrans(VerticesArrayEEPROM[currentMsgToSend], beaconID, qtyVertices)));
}

boolean analyzeReceivedData() {
  if (crudMsg.crudId == 1) {
    //check if exists in mem

    //check memory
    if (qtyVertices <= maxVertices) {
      Serial.println("Can add");
      //add vertice

      Serial.println(qtyVertices);
      VerticesArrayEEPROM[qtyVertices].verticeNumber = qtyVertices + 1;
      VerticesArrayEEPROM[qtyVertices].x = crudMsg.x;
      VerticesArrayEEPROM[qtyVertices].y = crudMsg.y;
      qtyVertices++;

      //get current free memory slot

      //write in eeprom


    }
    else  Serial.println("Beacon's memory is full.");
  }

  else if (crudMsg.crudId == 0) {
    Serial.println("Delete");

  }
  return true;
}

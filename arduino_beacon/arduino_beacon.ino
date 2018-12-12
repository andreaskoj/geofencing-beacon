// BEACON CODE - UNO

// Mode types; delete=0, add=1

// to write structures in eeprom
#include "EEPROMAnything.h"
#include "beaconLibrary.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

//constants
const uint8_t beaconID = 1;
const uint8_t maxVertices = 11; // MAX 10 vertices
const byte writeAddress[6] = "00002";
const byte readAddress[6] = "00001";

//declaring Array to store vertices in EEPROM
verticeInEEPROM verticesArrayEEPROM[maxVertices];

//received message struct
crudMsg crudMsg;

//struct to add new vertice to eeprom
verticeInEEPROM newEEPROMvertice;

//variables
int verticePos;
uint8_t freeMemorySlot;
uint8_t qtyVertices;
uint8_t currentMsgToSend;
boolean performingAnalyze = false;

//counters
uint8_t sendDataCounter = 0;

// method prototypes
void beaconMonolog();
void broadcastVertices();
boolean analyzeReceivedData();

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  // EEPROM DATA STRUCTURE
  // 1 * qtyVertices[1 byte]
  // 1 * VerticeInEEPROM[9 bytes]

  // check qty of verticies from EEPROM
  EEPROM_readAnything(0, qtyVertices);

  //get data from EEPROM
  for (int i = 0; i < qtyVertices; i++ ) {
    EEPROM_readAnything(sizeof(qtyVertices) + i * sizeof(verticeInEEPROM), verticesArrayEEPROM[i]);
  }

  // starting sending messages on nRF24L01
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(writeAddress);
  radio.openReadingPipe(0, readAddress);
  radio.startListening();

  //fillEEPROM();

  beaconMonolog();
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
      //Serial.println(crudMsg.x);
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

  radio.write(&msgToTrans(verticesArrayEEPROM[currentMsgToSend], beaconID, qtyVertices),
              sizeof(msgToTrans(verticesArrayEEPROM[currentMsgToSend], beaconID, qtyVertices)));
}

boolean analyzeReceivedData() {

  //check if a vertice exists in mem, if not set perfromAnalyze flag to 0
  if (crudMsg.crudId == 1 && checkIfexists(crudMsg, verticesArrayEEPROM , qtyVertices) >= 0 ) {

    //check memory
    if (qtyVertices <= maxVertices) {

      //add vertice to the array
      verticesArrayEEPROM[qtyVertices].verticeNumber = qtyVertices + 1;
      verticesArrayEEPROM[qtyVertices].x = crudMsg.x;
      verticesArrayEEPROM[qtyVertices].y = crudMsg.y;
      Serial.println("Verticle added to SRAM");

      // compute free EEPROM slot
      freeMemorySlot = qtyVertices * sizeof(verticeInEEPROM) + sizeof(qtyVertices);

      newEEPROMvertice.verticeNumber = qtyVertices + 1;
      newEEPROMvertice.x = crudMsg.x;
      newEEPROMvertice.y = crudMsg.y;

      //save vertice in eeprom
      EEPROM_writeAnything(freeMemorySlot , newEEPROMvertice);
      qtyVertices++;
      EEPROM_writeAnything(0 , qtyVertices);
      Serial.println("Verticle added to EEPROM");
    }
    else  Serial.println("Beacon's memory is full.");
  }

  else if (crudMsg.crudId == 0) {

    //get position from array
    verticePos = checkIfexists(crudMsg, verticesArrayEEPROM , qtyVertices);

    if (verticePos >= 0) {
      Serial.println("Delete a verticle -> " + (String)verticePos);


      //qtyVertices--;
    }
    else Serial.println("Vertice not found");
  }
  return true;
}

void beaconMonolog() {
  Serial.println("----------------------------");
  Serial.print("BeaconID   -> ");
  Serial.println(beaconID);
  Serial.print("Vertices   -> ");
  Serial.print(qtyVertices);
  Serial.print("/");
  Serial.println(maxVertices - 1);
  for (int i = 0; i < qtyVertices; i++)
    Serial.println("Vertice #" + (String)(i + 1) + " -> X=" + (String)verticesArrayEEPROM[i].x + " Y=" + (String)verticesArrayEEPROM[i].y);
  Serial.println("----------------------------");
}

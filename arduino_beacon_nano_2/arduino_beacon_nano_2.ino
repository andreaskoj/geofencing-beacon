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
const uint8_t beaconID = 2;
const uint8_t maxVertices = 11; // MAX 10 vertices
const byte writeAddress[6] = "00002";
const byte readAddress[6] = "00001";

//declaring Array to store vertices in EEPROM
verticeInEEPROM verticesArrayEEPROM[maxVertices];

//received message struct
editMsg editMsg;

//struct to add new vertice to eeprom
verticeInEEPROM newEEPROMvertice;

//variables

uint8_t freeMemorySlot;
uint8_t qtyVertices;
uint8_t currentMsgToSend;
boolean performingAnalyze = false;

int editAction = -2;

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

  // this function is set eeprom memory to 4 default vertices
  //fillEEPROMtoReset();

  beaconMonolog();
}

void loop() {
  sendData();
  delay(250);
  receiveData();
  delay(250);
  beaconMonolog();
}

void sendData() {
  radio.stopListening();
  broadcastVertices();
  radio.startListening();
}

void receiveData() {
  if (radio.available()) {
    if (performingAnalyze == false) {
      radio.read(&editMsg, sizeof(editMsg));
      
      editAction = pointInCirlce(editMsg.x , editMsg.y, qtyVertices, verticesArrayEEPROM );
      Serial.println(editAction);
    
      performingAnalyze = true;
      performingAnalyze = analyzeReceivedData();

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
  Serial.print("editAction");
  Serial.println(editAction);

  //check if a vertice exists in mem, if not set perfromAnalyze flag to 0
  if (editAction == -1) {

    //check memory
    if (qtyVertices <= maxVertices) {
      Serial.println("Caught add msg!");

      //add vertice to the array
      verticesArrayEEPROM[qtyVertices].verticeNumber = qtyVertices + 1;
      verticesArrayEEPROM[qtyVertices].x = editMsg.x;
      verticesArrayEEPROM[qtyVertices].y = editMsg.y;
      Serial.println("Verticle added to SRAM");

      // compute free EEPROM slot
      freeMemorySlot = qtyVertices * sizeof(verticeInEEPROM) + sizeof(qtyVertices);

      newEEPROMvertice.verticeNumber = qtyVertices + 1;
      newEEPROMvertice.x = editMsg.x;
      newEEPROMvertice.y = editMsg.y;

      //save vertice in eeprom
      EEPROM_writeAnything(freeMemorySlot , newEEPROMvertice);
      qtyVertices++;
      EEPROM_writeAnything(0 , qtyVertices);
      Serial.println("Verticle added to EEPROM");
    }
    else  Serial.println("Beacon's memory is full.");
  }

  else if (editAction >= 0) {

      Serial.println("Delete a verticle -> " + (String)editAction);

      // move this vecticle to the last postion of the array
      verticesArrayEEPROM[editAction].verticeNumber = qtyVertices + 1;

      // sort elements in array
      qsort(verticesArrayEEPROM, qtyVertices, sizeof(verticeInEEPROM), compareQuickSort);

      // set new numbers to the vertices
      for (uint8_t y = 0; y < qtyVertices; y++) {
        if (verticesArrayEEPROM[y].verticeNumber - (y + 1) != 0) {
          verticesArrayEEPROM[y].verticeNumber = (y + 1);
        }
      }
      //loose the reference to the last verticle
      qtyVertices--;

      // rewrite EEPROM 
      refreshEEPROM(qtyVertices, verticesArrayEEPROM );
      
  }
  return false;
}

void beaconMonolog() {
  Serial.println("----------------------------");
  Serial.print("BeaconID   -> ");
  Serial.println(beaconID);
  Serial.print("Vertices   -> ");
  Serial.print(qtyVertices);
  Serial.print("/");
  Serial.println(maxVertices - 1);
  Serial.println();
  for (int i = 0; i < qtyVertices; i++)
    Serial.println("Vertice #" + (String)(i + 1) + " -> X=" + (String)verticesArrayEEPROM[i].x + " Y=" + (String)verticesArrayEEPROM[i].y);
  Serial.println("----------------------------");
}

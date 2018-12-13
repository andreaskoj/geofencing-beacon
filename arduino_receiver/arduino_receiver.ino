// RECEIVER CODE - NANO

#include <nRF24L01.h>
#include <ArduinoJson.h>
#include <RF24.h>
#include <EEPROM.h>
#include <SPI.h>

RF24 radio(7, 8); // CE, CSN

struct VerticeMessage {
  uint8_t beaconId;
  uint8_t cVertice;
  uint8_t tVertices;
  float x;
  float y;
};

struct crudMsg {
  uint8_t beaconId;
  uint8_t crudId;
  float x;
  float y;
};


VerticeMessage testMsg;

struct VerticeInEEPROM {
  uint8_t verticeNumber;
  float x;
  float y;
};

crudMsg msg;

VerticeInEEPROM v1;
uint8_t addressEeprom = 0;
uint8_t value;
//address to listen
const byte readAddress[6] = "00002";

// address to send
const byte writeAddress[6] = "00001";
uint8_t counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;

  radio.begin();
  radio.openReadingPipe(0, readAddress);
  radio.openWritingPipe(writeAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {

  //    while(Serial.available() == 0);
  //  int val = Serial.read();
  //  Serial.println(val);
  receiveData();
  delay(250);
  sendData();
  delay(250);
}

void sendData() {
  // counter = counter % 5;

  // add = 0 , update = 1 , delete = 2
  //beaconID, crudID, x,x
  char test1[20] = "2,4,1,9.5369,5.7107";
  char test2[20] = "2,4,2,6.5369,9.7607";
  char test3[20] = "2,4,3,5.5369,4.7107";
  char test4[20] = "2,4,4,8.5369,9.7107";
  char test5[20] = "1,0,8.5369,9.7107";
  radio.stopListening();

  msg.beaconId = 1;
  msg.crudId = 0;

  msg.x = 3.434343;
  msg.y = 9.423424;

  //  msg.x = 0.542354;
  //  msg.y = 1.445264;

  //Serial.println(counter);
  //if (counter == 25)
 radio.write( &msg , sizeof(msg));

  //        if (counter == 0) radio.write( &test1 , sizeof(test1) );
  //        else if (counter == 1) radio.write( &test2 , sizeof(test2) );
  //        else if (counter == 2) radio.write( &test3 , sizeof(test3) );
  //        else if (counter == 3) radio.write( &test4 , sizeof(test4) );
  //        else radio.write( &test5 , sizeof(test5) );

  radio.startListening();
  counter ++;
}
void receiveData() {

  if (radio.available()) {

    radio.read(&testMsg, sizeof(testMsg));
    Serial.print(testMsg.beaconId);
    Serial.print(",");
    Serial.print(testMsg.cVertice);
    Serial.print(",");
    Serial.print(testMsg.tVertices);
    Serial.print(",");
    Serial.print(testMsg.x);
    Serial.print(",");
    Serial.println(testMsg.y);

  }
}

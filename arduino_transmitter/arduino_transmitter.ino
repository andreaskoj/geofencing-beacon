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
String msg;
int msgNumber;


void broadcastData();
void analyzeReceivedData();

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
    sendData();
    delay(250);
    receiveData();
    delay(250);
}

void sendData() {
        char test[20] = "Get from uno";
        radio.stopListening();
            //radio.write( &test , sizeof(test) );
            broadcastData();
        radio.startListening();

    }
    
void receiveData() {
      if (radio.available()) {
        char data[32] = "";
        radio.read(&data, sizeof(data));
        analyzeReceivedData(data);
      }     
}

void broadcastData() {
  
    //prevent to get minus numbers on counter
    if (counter < 0) {counter = 0;}
   
    counter++;
    
    // get number of verticle to send
    msgNumber = counter % qtyVerticles ;

    //preparing messages
    msg = String(deviceID)+","+String(9)+","+String(qtyVerticles)+","+String(verticlesArray[msgNumber].verticleNumber)+
    ","+String(verticlesArray[msgNumber].x)+","+String(verticlesArray[msgNumber].y);       

    //casting to array of chars
    msg.toCharArray(charBuffer, 32); 
     
    radio.write(&charBuffer, sizeof(charBuffer));
  }

void analyzeReceivedData(char *data) {

    String dataInString = String(data);
    String id;
    String crud;
    String x;
    String y;
    int speratorPosition;
    char separator = ',';

    // find position of the first semicolon, substring, delete, and agin

      
    if (dataInString.length() == 17) {
    
      speratorPosition = dataInString.indexOf(separator);
      id = dataInString.substring(0, speratorPosition);
      dataInString.remove(0,speratorPosition+1);

      if ( id.toInt() == deviceID)
       {
          //get crud 
          speratorPosition = dataInString.indexOf(separator);
          crud = dataInString.substring(0, speratorPosition);
          dataInString.remove(0,speratorPosition+1);

          //get x
          speratorPosition = dataInString.indexOf(separator);
          x = dataInString.substring(0, speratorPosition);
          dataInString.remove(0,speratorPosition+1);

          //get y
          y = dataInString;

       if (crud == "0") {
        Serial.println("Will be added");
        //add verticle 
       }
       else if (crud == "1") {
        //update verticle
        
       }
       else if (crud == "2") {
        //delete verticle
       }
        else 
        Serial.println("Wrong CRUD.");
                
       }
    }
}

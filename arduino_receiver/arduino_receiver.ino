// RECEIVER CODE - NANO

    #include <nRF24L01.h>
    #include <ArduinoJson.h>
    #include <RF24.h>
    #include <EEPROM.h>
    #include <SPI.h>

    RF24 radio(7, 8); // CE, CSN
 
    int addressEeprom = 0;
    byte value;
    const byte readAddress[6] = "00001";
    const byte writeAddress[6] = "00002";
    
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
    receiveData();
    delay(250);
    sendData();
    delay(250);
    }




    void sendData() {
      // add = 0 , update = 1 , delete = 2
      //beaconID, crudID, x,x
      
        char test[20] = "0,0,9.5369,9.7107";
        radio.stopListening();
            radio.write( &test , sizeof(test) );
            Serial.println("**************SENT");
        radio.startListening();

    }
void receiveData() {

      if (radio.available()) {
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.println(text);
      }
}

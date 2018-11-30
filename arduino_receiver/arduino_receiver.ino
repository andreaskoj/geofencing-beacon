// RECEIVER CODE - NANO

    #include <nRF24L01.h>
    #include <ArduinoJson.h>
    #include <RF24.h>
    #include <EEPROM.h>
    #include <SPI.h>

    RF24 radio(7, 8); // CE, CSN
 
    int addressEeprom = 0;
    byte value;
    const byte address[6] = "00001";
    
     void setup() {
      Serial.begin(9600);
      while (!Serial) continue;
      
      radio.begin();
      radio.openReadingPipe(0, address);
      radio.setPALevel(RF24_PA_MIN);
      radio.startListening();
    }
    void loop() {
      if (radio.available()) {
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.println(text);
        
      }
    }

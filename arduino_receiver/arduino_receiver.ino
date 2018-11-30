

// RECEIVER CODE - Arduino Nano
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

      const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(3);
      DynamicJsonBuffer jsonBuffer(bufferSize);
      
      JsonObject& root = jsonBuffer.createObject();
      root["name"] = "beacon1";
      
      JsonArray& verticles = root.createNestedArray("verticles");
      
      JsonObject& verticles_0 = verticles.createNestedObject();
      verticles_0["number"] = 1;
      verticles_0["longtitude"] = "6.88";
      verticles_0["langtitude"] = "5.77";
      
      JsonObject& verticles_1 = verticles.createNestedObject();
      verticles_1["number"] = 1;
      verticles_1["longtitude"] = "9.88";
      verticles_1["langtitude"] = "2.77";
      
      root.printTo(Serial);
        
      //EEPROM.write(addressEeprom, 123);
      value = EEPROM.read(addressEeprom);
      Serial.println(EEPROM.length());
      Serial.println(value, DEC); 
      
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

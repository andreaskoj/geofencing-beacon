#define PIP_CIRCLE_RADIUS 0.00001f
// change to verticesEEPROM
struct verticeInEEPROM {
  uint8_t verticeNumber;
  float x;
  float y;
};

//change to broadcastMessage
struct verticeMessage {
  uint8_t beaconId;
  uint8_t cVertice;
  uint8_t tVertices;
  float x;
  float y;
};

struct editMsg {
  float x;
  float y;
};

// message to send struct
verticeMessage msg;

verticeMessage& msgToTrans(verticeInEEPROM& vertice, const uint8_t& beaconId, const uint8_t& qtyVertices) {

  msg.beaconId = beaconId;
  msg.cVertice = vertice.verticeNumber;
  msg.tVertices = qtyVertices;
  msg.x = vertice.x;
  msg.y = vertice.y;

  return msg;
}


// check memory to find the same vertice

int checkIfexists (editMsg & editMsg , verticeInEEPROM verticesArrayEEPROM  [ ], const uint8_t& qtyVertices) {

  // traverse the array and if find the same pair X & Y return true else false
  for ( int i = 0; i < qtyVertices; i++) {

    if (editMsg.x == verticesArrayEEPROM[i].x && editMsg.y == verticesArrayEEPROM[i].y) {
      Serial.println( "Found this verticle in memory on position -> " + (String)(i + 1) );
      return i;
    }
  }
  return -1;
}

int compareQuickSort (const void * a, const void * b) {

  verticeInEEPROM *verticeInEEPROM1 = (verticeInEEPROM *)a;
  verticeInEEPROM *verticeInEEPROM2 = (verticeInEEPROM *)b;

  return ( verticeInEEPROM1 -> verticeNumber - verticeInEEPROM2 -> verticeNumber );
}

void beaconCommunication() {
  //  Serial.println("Welcome to Beacon's serial menu.");
  //  Serial.println("Choose on of the follwoing options:");
  //  while (Serial.available() == 0);
  //  int val = Serial.read();
  //  Serial.println(val);
}

void refreshEEPROM(uint8_t& qtyVertices, verticeInEEPROM verticesArrayEEPROM[]) {

  // write number of vertices to EEPROM
  EEPROM_writeAnything(0 , qtyVertices );

  for (int i = 0; i < qtyVertices; i++) {
    EEPROM_writeAnything(sizeof(qtyVertices) + sizeof(verticeInEEPROM) * i , verticesArrayEEPROM[i] );
  }
}

int pointInCirlce(float xMiddle, float yMiddle, uint8_t& qtyVertices, verticeInEEPROM verticesArrayEEPROM[]  ) {

  // PIP_CIRCLE_RADIUS 0.00001f
  // Pythagorean theorem
  Serial.println("-------------------");
  for ( int i = 0; i < qtyVertices; i++) {
    if (sqrt((pow((abs(xMiddle - verticesArrayEEPROM[i].x)), 2)) + (pow((abs(yMiddle - verticesArrayEEPROM[i].y)), 2))) < PIP_CIRCLE_RADIUS){
      Serial.print("In cricle -> ");
      Serial.println(i);
      return i;
  }
    else {
      Serial.print("Outside cricle -> ");
      Serial.println(i);
    }
  }


  return -1;
}

void fillEEPROMtoReset () {

  uint8_t qtyVertices = 4;

  verticeInEEPROM v1, v2, v3, v4;
  
//Location 1
//Vertex 1; x = 11.978166, y = 57.687432
//Vertex 2; x = 11.978432, y = 57.687485
//Vertex 3; x = 11.978548, y = 57.687361
//Vertex 4; x = 11.978272, y = 57.687307
//
//Location 2
//Vertex 1; x = 11.980697, y = 57.687899
//Vertex 2; x = 11.980839, y = 57.687743
//Vertex 3; x = 11.981332, y = 57.687865
//Vertex 4; x = 11.981185, y = 57.688023

  // Location 1 
  v1.verticeNumber = 1;
  v1.x = 57.687432;
  v1.y = 11.978166;

  v2.verticeNumber = 2;
  v2.x = 57.687485;
  v2.y = 11.978432;

  v3.verticeNumber = 3;
  v3.x = 57.687361;
  v3.y = 11.978548;

  v4.verticeNumber = 4;
  v4.x = 57.687307;
  v4.y = 11.978272;

  Serial.println(" TEST - size of verticle -> "  + (String)sizeof(v1));
  Serial.println(" TEST2 - size of first element in EEPROM ->  "  + (String)sizeof(qtyVertices));

  EEPROM_writeAnything(0 , qtyVertices );
  EEPROM_writeAnything(1 , v1);
  EEPROM_writeAnything(1 + sizeof(v1), v2);
  EEPROM_writeAnything(1 + sizeof(v1) * 2, v3);
  EEPROM_writeAnything(1 + sizeof(v1) * 3, v4);

  Serial.println("Added initial set of vertices");

}

struct verticeInEEPROM {
  uint8_t verticeNumber;
  float x;
  float y;
};

struct verticeMessage {
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

int checkIfexists(crudMsg& crudMsg, verticeInEEPROM verticesArrayEEPROM[], const uint8_t& qtyVertices) {

  // traverse the array and if find the same pair X & Y return true else false
  for ( int i = 0; i < qtyVertices; i++) {

    if (crudMsg.x == verticesArrayEEPROM[i].x && crudMsg.y == verticesArrayEEPROM[i].y) {
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


void fillEEPROMtoReset () {

  uint8_t qtyVertices = 4;

  verticeInEEPROM v1, v2, v3, v4;

  v1.verticeNumber = 1;
  v1.x = 5.453454;
  v1.y = 7.353455;

  v2.verticeNumber = 2;
  v2.x = 1.334234;
  v2.y = 3.335434;

  v3.verticeNumber = 3;
  v3.x = 5.342354;
  v3.y = 2.324524;

  v4.verticeNumber = 4;
  v4.x = 3.434343;
  v4.y = 9.423424;

  Serial.println(" TEST - size of verticle -> "  + (String)sizeof(v1));
  Serial.println(" TEST2 - size of first element in EEPROM ->  "  + (String)sizeof(qtyVertices));

  EEPROM_writeAnything(0 , qtyVertices );
  EEPROM_writeAnything(1 , v1);
  EEPROM_writeAnything(1 + sizeof(v1), v2);
  EEPROM_writeAnything(1 + sizeof(v1) * 2, v3);
  EEPROM_writeAnything(1 + sizeof(v1) * 3, v4);

  Serial.println("Added initial set of vertices");

}

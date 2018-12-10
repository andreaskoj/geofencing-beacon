struct VerticeInEEPROM {
  unsigned int verticeNumber;
  float x;
  float y;
};

struct VerticeMessage {
  unsigned int beaconId;
  unsigned int cVertice;
  unsigned int tVertices;
  float x;
  float y;
};

struct crudMsg{
  unsigned int beaconId;
  unsigned int crudId;
  float x;
  float y;  
};



// message to send struct
VerticeMessage msg;

VerticeMessage& msgToTrans(VerticeInEEPROM vertice, unsigned int beaconId, unsigned int qtyVertices) {

  msg.beaconId = beaconId;
  msg.cVertice = vertice.verticeNumber;
  msg.tVertices = qtyVertices;
  msg.x = vertice.x;
  msg.y = vertice.y;

  return msg;
}

void fillEEPROM () {

  unsigned int qtyVertices = 4;

  VerticeInEEPROM v1, v2, v3, v4;

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

  Serial.println(sizeof(v1));

  EEPROM_writeAnything(0 , qtyVertices );
  EEPROM_writeAnything(2 , v1);
  EEPROM_writeAnything(2 + 10, v2);
  EEPROM_writeAnything(2 + 10 * 2, v3);
  EEPROM_writeAnything(2 + 10 * 3, v4);
}

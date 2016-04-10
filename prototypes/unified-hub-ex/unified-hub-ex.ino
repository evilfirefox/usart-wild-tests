#include <SoftwareSerial.h>

#define BAUDRATE 19200
#define PIN_RX_MASTER 4
#define PIN_TX_MASTER 5
#define PIN_RX_SLAVE 6
#define PIN_TX_SLAVE 7
#define PIN_ID_DATA 9
#define PIN_ID_CLOCK 8
#define PIN_ID_LATCH 10
#define PACKET_LENGTH 4

struct Response {
  byte id;
  byte dataSize;
  byte *dataPtr;
};

SoftwareSerial master = SoftwareSerial(PIN_RX_MASTER, PIN_TX_MASTER);
SoftwareSerial slave = SoftwareSerial(PIN_RX_SLAVE, PIN_TX_SLAVE);
byte hubId = 0;
byte command[PACKET_LENGTH];
byte bytesRead = 0, bytesReadSlave = 0;
struct Response *list;

void setup() {
  pinMode(PIN_RX_MASTER, INPUT);
  pinMode(PIN_RX_SLAVE, INPUT);
  pinMode(PIN_ID_DATA, INPUT);
  pinMode(PIN_TX_MASTER, OUTPUT);
  pinMode(PIN_TX_SLAVE, OUTPUT);
  pinMode(PIN_ID_CLOCK, OUTPUT);
  pinMode(PIN_ID_LATCH, OUTPUT);

  Serial.begin(BAUDRATE);
  while (!Serial) {   
  }
  
  master.begin(BAUDRATE);
  slave.begin(BAUDRATE);

  digitalWrite(PIN_ID_LATCH, LOW);
  digitalWrite(PIN_ID_LATCH, HIGH);
  hubId = shiftIn(PIN_ID_DATA, PIN_ID_CLOCK, LSBFIRST);
  Serial.write(hubId);
}

void loop() {  
  master.listen();
  if (master.available()) {
    int id = master.read();
    Serial.println(id);
    if (id != hubId) {     
      return;
    }
    bytesRead = master.readBytes(command, PACKET_LENGTH);
    int i = 0;
    for (i = 0; i < 3; i++) {
      slave.write(i);
      slave.write(command, bytesRead);
      slave.listen();
      while (!slave.available()){      
      }
      struct Response *response = (Response*) malloc(sizeof(Response));
      response->id = slave.read();
      response->dataSize = slave.read();
      byte data[response->dataSize];
      bytesReadSlave = slave.readBytes(data, response->dataSize);
      if (bytesReadSlave != response->dataSize) {
        Serial.write("size packet does not match");
      }
      response->dataPtr = data;
      list = response;
    }
    /*for (i = 0; i < 3; i++) {
      master.write(i);
      master.write(response[i], PACKET_LENGTH);
    }*/
  }
}




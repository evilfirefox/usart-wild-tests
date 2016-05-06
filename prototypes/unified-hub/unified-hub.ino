#include <SoftwareSerial.h>

#define BAUDRATE 9600
#define PIN_RX_MASTER 4
#define PIN_TX_MASTER 5
#define PIN_RX_SLAVE 6
#define PIN_TX_SLAVE 7
#define PIN_DIRECTION_MASTER 8
#define PIN_DIRECTION_SLAVE 9
/*#define PIN_ID_DATA 9
  #define PIN_ID_CLOCK 8
  #define PIN_ID_LATCH 10*/
#define PACKET_LENGTH 4

SoftwareSerial master = SoftwareSerial(PIN_RX_MASTER, PIN_TX_MASTER);
SoftwareSerial slave = SoftwareSerial(PIN_RX_SLAVE, PIN_TX_SLAVE);
byte hubId = 0;
byte command[PACKET_LENGTH];
byte bytesRead = 0, bytesReadSlave = 0;
byte response[8][PACKET_LENGTH];

void setup() {
  pinMode(PIN_RX_MASTER, INPUT);
  pinMode(PIN_RX_SLAVE, INPUT);
  // pinMode(PIN_ID_DATA, INPUT);
  pinMode(PIN_TX_MASTER, OUTPUT);
  pinMode(PIN_TX_SLAVE, OUTPUT);
  //  pinMode(PIN_ID_CLOCK, OUTPUT);
  //  pinMode(PIN_ID_LATCH, OUTPUT);
  pinMode(PIN_DIRECTION_MASTER, OUTPUT);
  pinMode(PIN_DIRECTION_SLAVE, OUTPUT);

  Serial.begin(BAUDRATE);
  while (!Serial) {
  }

  master.begin(BAUDRATE);
  slave.begin(BAUDRATE);

  /*digitalWrite(PIN_ID_LATCH, LOW);
    digitalWrite(PIN_ID_LATCH, HIGH);
    hubId = shiftIn(PIN_ID_DATA, PIN_ID_CLOCK, LSBFIRST);*/
  hubId = 1;
  Serial.println(hubId);
  digitalWrite(PIN_DIRECTION_MASTER, LOW);
  master.listen();
  Serial.println("Listening to master");
}

void loop() {
  if (master.available()) {
    int id = master.read();
    Serial.print("master id recieved: ");
    Serial.println(id);
    if (id != hubId) {
      return;
    }
    bytesRead = master.readBytes(command, PACKET_LENGTH);
    Serial.print("master transmission complete: ");
    Serial.println(bytesRead);
    Serial.write(command, PACKET_LENGTH);
    int i = 0;
    for (i = 0; i < 1; i++) {
      Serial.print("addressing slave: ");
      Serial.println(i);
      digitalWrite(PIN_DIRECTION_SLAVE, HIGH);
      slave.write(i);
      slave.write(command, bytesRead);
      digitalWrite(PIN_DIRECTION_SLAVE, LOW);
      slave.listen();
      while (!slave.available()) {
        Serial.print(".");
        delay(1000);
      }
      int slaveId = slave.read();
      bytesReadSlave = slave.readBytes(response[slaveId], PACKET_LENGTH);
    }
    Serial.println("slave response recieved");

    Serial.println("Upstreaming results");
    digitalWrite(PIN_DIRECTION_MASTER, HIGH);
    for (i = 0; i < 1; i++) {
      Serial.println(i);
      master.write(i);
      master.write(response[i], PACKET_LENGTH);
    }
    digitalWrite(PIN_DIRECTION_MASTER, LOW);
    Serial.println("transmission complete");
    master.listen();
  }
}


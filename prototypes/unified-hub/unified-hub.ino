#include <SoftwareSerial.h>

#define PIN_RX_MASTER 4
#define PIN_TX_MASTER 5
#define PIN_RX_SLAVE 6
#define PIN_TX_SLAVE 7
#define PIN_ID_DATA 8
#define PIN_ID_CLOCK 9
#define PACKET_LENGTH 4

SoftwareSerial master = SoftwareSerial(PIN_RX_MASTER, PIN_TX_MASTER);
SoftwareSerial slave = SoftwareSerial(PIN_RX_SLAVE, PIN_TX_SLAVE);
byte hubId = 0;
byte command[PACKET_LENGTH];
byte bytesRead = 0;

void setup() {
  pinMode(PIN_RX_MASTER, INPUT);
  pinMode(PIN_RX_SLAVE, INPUT);
  pinMode(PIN_TX_MASTER, OUTPUT);
  pinMode(PIN_TX_SLAVE, OUTPUT);

  Serial.begin(4800);
  while (!Serial) {   
  }
  
  master.begin(9600);
  slave.begin(9600);

  hubId = shiftIn(PIN_ID_DATA, PIN_ID_CLOCK, MSBFIRST);
}

void loop() {
  master.listen();
  if (master.available()) {
     bytesRead = master.readBytes(command, PACKET_LENGTH);
     slave.write(command, bytesRead);
  }
  delay(1000);
}

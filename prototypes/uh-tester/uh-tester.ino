#include <SoftwareSerial.h>

#define BAUDRATE 9600
#define PIN_RX_MASTER 4
#define PIN_TX_MASTER 5
#define PIN_RX_SLAVE 6
#define PIN_TX_SLAVE 7
#define PIN_DIRECTION_MASTER 8
#define PIN_DIRECTION_SLAVE 9
/* #define PIN_ID_DATA 9
#define PIN_ID_CLOCK 8
#define PIN_ID_LATCH 10*/
#define PACKET_LENGTH 4

SoftwareSerial master = SoftwareSerial(PIN_RX_MASTER, PIN_TX_MASTER);
SoftwareSerial slave = SoftwareSerial(PIN_RX_SLAVE, PIN_TX_SLAVE);
byte data[PACKET_LENGTH];
byte bytesRead = 0, bytesReadSlave = 0;

void setup() {
  pinMode(PIN_RX_MASTER, INPUT);
  pinMode(PIN_RX_SLAVE, INPUT);
  pinMode(PIN_TX_MASTER, OUTPUT);
  pinMode(PIN_TX_SLAVE, OUTPUT);
  pinMode(PIN_DIRECTION_MASTER, OUTPUT);
  pinMode(PIN_DIRECTION_SLAVE, OUTPUT);
  
  Serial.begin(BAUDRATE);
  while (!Serial) {   
  }
  
  slave.begin(BAUDRATE);
  Serial.println("MasterTest started");
}

void loop() {  
  if (Serial.available()){
    bytesRead = Serial.readBytes(data, PACKET_LENGTH);
    Serial.println(bytesRead);
    digitalWrite(PIN_DIRECTION_SLAVE, HIGH);
    slave.write(1);
    slave.write(data, bytesRead);
    digitalWrite(PIN_DIRECTION_SLAVE, LOW);
    Serial.println("Transmission complete");    
    slave.listen();
    while(!slave.available()){      
    }
    byte slaveId = slave.read();
    bytesReadSlave = slave.readBytes(data, PACKET_LENGTH);
    Serial.write(slaveId);
    Serial.write(data, bytesReadSlave);
  }

}

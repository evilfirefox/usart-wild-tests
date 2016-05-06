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
byte slaveId = 0;
byte data[PACKET_LENGTH];
byte bytesRead = 0, bytesReadSerial = 0;

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

  master.begin(BAUDRATE);
  Serial.println("SlaverTest started");
}

void loop() {
  if (master.available()) {
    int id = master.read();
    Serial.print("master id recieved: ");
    Serial.println(id);
    if (id != slaveId) {
      Serial.print("Id check fails");
      Serial.println(id);
      return;
    }
    bytesRead = master.readBytes(data, PACKET_LENGTH);
    Serial.print("master transmission complete: ");
    Serial.println(bytesRead);
    Serial.write(data, PACKET_LENGTH);
    while (!Serial.available()) {      
    }
    bytesReadSerial = Serial.readBytes(data, PACKET_LENGTH);
    digitalWrite(PIN_DIRECTION_MASTER, HIGH);
    master.write(slaveId);
    master.write(data, PACKET_LENGTH);
    digitalWrite(PIN_DIRECTION_MASTER, LOW);
    Serial.println("packet sent");
  }

}

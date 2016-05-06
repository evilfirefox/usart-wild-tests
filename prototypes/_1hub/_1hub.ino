#include <enc28j60.h>
#include <EtherCard.h>
#include <net.h>
#include <SoftwareSerial.h>

#define BAUDRATE 9600
#define PIN_RX_SLAVE 6
#define PIN_TX_SLAVE 7
#define PIN_DIRECTION_SLAVE 9
#define PACKET_LENGTH 4

static byte ip[] = {192, 168, 33, 39};
static byte gw[] = {192, 168, 33, 1};
static byte dns[] = {192, 168, 33, 201};
static byte mac[] = {0x33, 0x99, 0x77, 0x97, 0xDD, 0xEE};

byte Ethernet::buffer[500];
SoftwareSerial slave = SoftwareSerial(PIN_RX_SLAVE, PIN_TX_SLAVE);
char data[PACKET_LENGTH];
byte bytesRead = 0, bytesReadSlave = 0;

char responseBuffer[58];
String response =
  "HTTP/1.0 200 Successful\r\n"
  "Content-Type: text/plain\r\n"
  "\r\n";

void setup() {
  Serial.begin(9600);
  if (ether.begin(sizeof(Ethernet::buffer), mac) == 0) {
    Serial.println("Ethernet initialization error");
  }

  if (ether.staticSetup(ip, gw, dns) == 0) {
    Serial.println("Unable to setup static settings");
  }

  pinMode(PIN_RX_SLAVE, INPUT);
  pinMode(PIN_TX_SLAVE, OUTPUT);
  pinMode(PIN_DIRECTION_SLAVE, OUTPUT);
  slave.begin(BAUDRATE);
  Serial.println("1hub started");
}

void loop() {
  word pos;
  if ((pos = ether.packetLoop(ether.packetReceive()))) {
    char* request = (char *) Ethernet::buffer + pos;
    Serial.println(request);
    interactWSlave();
    String stringData = String(data);
    response.concat(stringData);
    Serial.println(response);
    response.toCharArray(responseBuffer, response.length());
    memcpy(ether.tcpOffset(), responseBuffer, sizeof responseBuffer);
    ether.httpServerReply(sizeof responseBuffer - 1);
    Serial.println("response sent");
  }
}

void interactWSlave() {
  digitalWrite(PIN_DIRECTION_SLAVE, HIGH);
  slave.write(1);
  slave.write(data, bytesRead);
  digitalWrite(PIN_DIRECTION_SLAVE, LOW);
  Serial.println("Transmission complete");
  slave.listen();
  while (!slave.available()) {
    Serial.print(".");
    delay(1000);
  }
  byte slaveId = slave.read();
  bytesReadSlave = slave.readBytes(data, PACKET_LENGTH);
  Serial.write(slaveId);
  Serial.write(data, bytesReadSlave);
}

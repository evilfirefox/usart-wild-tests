#include <enc28j60.h>
#include <EtherCard.h>
#include <net.h>

static byte ip[] = {192, 168, 33, 39};
static byte gw[] = {192, 168, 33, 1};
static byte dns[] = {192, 168, 33, 201};
static byte mac[] = {0x33, 0x99, 0x77, 0x97, 0xDD, 0xEE};

byte Ethernet:: buffer[500];

const char page[] PROGMEM =
  "HTTP/1.0 200 Successful\r\n"
  "Content-Type: text/html\r\n"
  "\r\n"
  "<html><body><h3>arduino server here...</h3></body></html>";

void setup() {
  Serial.begin(9600);
  if (ether.begin(sizeof(Ethernet::buffer), mac) == 0) {
    Serial.println("Ethernet initialization error");
  }

  if (ether.staticSetup(ip, gw, dns) == 0) {
    Serial.println("Unable to setup static settings");
  }

  Serial.println("Ethernet started");
}

void loop() {
  if (ether.packetLoop(ether.packetReceive())) {
    Serial.write(Ethernet::buffer, sizeof(Ethernet::buffer));
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}

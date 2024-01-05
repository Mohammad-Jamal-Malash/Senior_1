#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h>
#include "binary.h"
#include <DHT.h>

#define ssRX  9 /* Rx pin for software serial */
#define ssTX  8 /* Tx pin for software serial */

DHT dht(4,DHT22);


/* Create object named xbee_ss of the class SoftwareSerial */ 
SoftwareSerial xbee_ss(ssRX, ssTX); /* Define pins for software serial instance named xbee-ss(any name of your choice) to be connected to xbee */
/* ssTx of Arduino connected to Din (pin 3 of xbee) */
/* ssRx of Arduino connected to Dout (pin 2 of xbee) */

XBeeWithCallbacks xbee; /* Create an object named xbee(any name of your choice) of the class XBee */


void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  xbee_ss.begin(9600); /* Define baud rate for software serial communication */
  xbee.setSerial(xbee_ss); /* Define serial communication to be used for communication with xbee */
  /* In this case, software serial is used. You could use hardware serial as well by writing "Serial" in place of "xbee_ss" */
  /* For UNO, software serialis required so that we can use hardware serial for debugging and verification */
  /* If using a board like Mega, you can use Serial, Serial1, etc. for the same, and there will be no need for software serial */

    delay(1);

  Serial.println(F("Starting..."));


 xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&Serial);
 xbee.onResponse(printErrorCb, (uintptr_t)(Print*)&Serial);


  // Setup DHT sensor
  dht.begin();

  // Send a first packet right away
  sendPacket();

}

void sendPacket() {
 // Prepare the Zigbee Transmit Request API packet
  ZBTxRequest txRequest;
 txRequest.setAddress64(0x0000000000000000);
  txRequest.setAddress16(0x0000);
 // Allocate 9 payload bytes: 1 type byte plus two floats of 4  bytes each
  AllocBuffer<9> packet;

 // Packet type, temperature, humidity
 packet.append<uint8_t>(1);
 packet.append<float>(dht.readTemperature());
 packet.append<float>(dht.readHumidity());
 txRequest.setPayload(packet.head, packet.len());
 
 // And send it
 xbee.send(txRequest);
}


unsigned long last_tx_time = 0;

void loop() {
  // Check the serial port to see if there is a new packet available
  xbee.loop();

  // Send a packet every 10 seconds
  if (millis() - last_tx_time > 10000) {
    sendPacket();
    last_tx_time = millis();
  }
}



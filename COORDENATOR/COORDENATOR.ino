#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h>
#include "binary.h"


#define ssRX  9 /* Rx pin for software serial */
#define ssTX  8 /* Tx pin for software serial */



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
 xbee.onZBRxResponse(processRxPacket);

}

void processRxPacket(ZBRxResponse& rx, uintptr_t) {
  Buffer b(rx.getData(), rx.getDataLength());
  uint8_t type = b.remove<uint8_t>();

  if (type == 1 && b.len() == 8) {
    Serial.print(F("DHT packet received from "));
    printHex(Serial, rx.getRemoteAddress64());
    Serial.println();
    Serial.print(F("Temperature: "));
    Serial.println(b.remove<float>());
    Serial.print(F("Humidity: "));
    Serial.println(b.remove<float>());
    return;
  }

  Serial.println(F("Unknown or invalid packet"));
  printResponse(rx, Serial);
}


void loop() {
  // Check the serial port to see if there is a new packet available
  xbee.loop();
}



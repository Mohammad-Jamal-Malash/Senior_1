#include <LinkedList.h>
#include <XBee.h>
#include <Printers.h>
#include <SoftwareSerial.h>
#include "binary.h"
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>


const char *ssid =  "JAMAL";     // replace with your wifi ssid and wpa2 key
const char *pass =  "1234554321";


const char MQTT_SERVER[] PROGMEM = "mqtt.beebotte.com";
const char MQTT_CLIENTID[] PROGMEM = "";
const char MQTT_USERNAME[] PROGMEM = "1BBve2k2DsdqOmzAtlOBpVzi0uVH5qcW";
const char MQTT_PASSWORD[] PROGMEM = "";
const int MQTT_PORT = 1883;


WiFiClient client;


Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT,
 MQTT_CLIENTID,MQTT_USERNAME, MQTT_PASSWORD);


LinkedList<XBeeAddress64> NodesList;

#define ssRX  9 /* Rx pin for software serial */
#define ssTX  8 /* Tx pin for software serial */



/* Create object named xbee_ss of the class SoftwareSerial */ 
SoftwareSerial xbee_ss(ssRX, ssTX); /* Define pins for software serial instance named xbee-ss(any name of your choice) to be connected to xbee */
/* ssTx of Arduino connected to Din (pin 3 of xbee) */
/* ssRx of Arduino connected to Dout (pin 2 of xbee) */

XBeeWithCallbacks xbee; /* Create an object named xbee(any name of your choice) of the class XBee */


//////////////////////////////////////////////////////////////

void publish(const __FlashStringHelper *resource, float value) {

  const char* resourceStr = reinterpret_cast<const char*>(resource);
  // Use JSON to wrap the data, so Beebotte will remember the data
  // (instead of just publishing it to whoever is currently listening).
  String data;
  data += "{\"data\": ";
  data += value;
  data += ", \"write\": true}";

  Serial.print(F("Publishing "));
  Serial.print(data);
  Serial.print(F( " to "));
  Serial.println(resource);

 // Publish data and try to reconnect when publishing data fails
  if (!mqtt.publish(resourceStr, data.c_str())) {
    Serial.println(F("Failed to publish, trying reconnect..."));
    connect();
  }
   /* if (!mqtt.publish(resourceStr, data.c_str()))
      Serial.println(F("Still failed to publish data"));
  }*/
}
///////////////////////////////////////////////////////////////
int indexOf(XBeeAddress64 addr){
    int index = -1;
    for (int i = 0; i < NodesList.size(); i++) {
    if (NodesList.get(i) == addr) {
        index = i;
        break;
    }
}

  return index;
}

///////////////////////////////////////////////////////////////

void processRxPacket(ZBRxResponse& rx, uintptr_t) {
  
  XBeeAddress64 addr = rx.getRemoteAddress64();
  int index =   indexOf(addr);
  String Node = String(addr,HEX);

    Buffer b(rx.getData(), rx.getDataLength());
    uint8_t type = b.remove<uint8_t>();

      if(index == -1){
        NodesList.add(addr);
      }

      if ( type == 1 && b.len() == 8) {
      publish(FPSTR((Node +"/Temperature").c_str()), b.remove<float>());
      publish(FPSTR((Node +"/Humidity").c_str()), b.remove<float>());
      return;
  }

  Serial.println(F("Unknown or invalid packet"));
  printResponse(rx, Serial);
}

///////////////////////////////////////////////////////////////
void connect() {
  client.stop(); // Ensure any old connection is closed
  uint8_t ret = mqtt.connect();
  if (ret == 0)
    Serial.println(F("MQTT connected"));
  else
    Serial.println(mqtt.connectErrorString(ret));
}

//////////////////////////////////////////////////////////////////


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



void loop() {
  // Check the serial port to see if there is a new packet available
  xbee.loop();
}



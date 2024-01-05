#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <SPI.h>
#include "binary.h"


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



void connect() {
  client.stop(); // Ensure any old connection is closed
  uint8_t ret = mqtt.connect();
  if (ret == 0)
    Serial.println(F("MQTT connected"));
  else
    Serial.println(mqtt.connectErrorString(ret));
}






float TEMP = 10.9;
float HUM = 30.7; 
 
void setup() 
{
       Serial.begin(9600);
       delay(10);
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

      connect();

 
}
 unsigned long previousMillis = 0;
const long interval = 1000;  // Delay in milliseconds
void loop() 
{
  TEMP++;
  TEMP++;
          publish(F("END_Device/TEMP"), TEMP);
      publish(F("END_Device/HUM"), HUM);


      float h = TEMP--;
      float t = HUM++;
      
         delay(10000);
         

 

}
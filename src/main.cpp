// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include<Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include "connections.h"
#include"ThingSpeak.h"
#include<PubSubClient.h>


#define DHTPIN D2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);
WiFiClient  client;
uint32_t delayMS;
Parameters params;
PubSubClient mq_client(params.mqtt_server,1883, client);

void setup() {
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  delayMS = sensor.min_delay / 1000;
  Serial.print(delayMS);


  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(params.ssid,params.password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);     
  } 
  Serial.println("\nConnected.");

  String clientName="ESP-Thingspeak";
  Serial.print("Connecting to ");
  Serial.print(params.mqtt_server);
  Serial.print(" as ");
  Serial.println(clientName);
  
 

  if (mq_client.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(params.topic);
    
    if (mq_client.publish(params.topic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }

}
  


int return_code=0;

void loop() {

  float temperature;
  
  delay(15500);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    temperature = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));
    pushDatamqtt(1,temperature,params,&mq_client);

   /*
    return_code = ThingSpeak.writeField(params.ChannelID, 1, temperature, params.writeAPIKey);
    
    
    Serial.print("return_code "+ String(return_code));
    if(return_code == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(return_code));
    }*/
   // for (int t=0;t<1000;t++)
  //  pushData(1,temperature,params); 
  }
  
  /*
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
*/

}
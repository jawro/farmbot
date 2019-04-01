#include "connections.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Parameters::Parameters(const char * ssid, const char * password, const char * host, const char * writeAPIKey)
{
    this->host = host;
    this->password = password;
    this->ssid = ssid;
    this->writeAPIKey = writeAPIKey;   
}

Parameters::Parameters()
{
    
}

int pushData(short field, float data,const Parameters & params)
{
   if ((millis() % (100)) == 0) {
// make TCP connections
WiFiClient client;
const int httpPort = 80;

if (!client.connect(params.host, httpPort)) 
    return -1;

String url = "/update?key=";
url+=params.writeAPIKey;
url+="&field";
url+=String(field);
url+='=';
url+=String(data);
url+="\r\n";
//Serial.println(url);
// Request to the server
client.print(String("GET ") + url + " HTTP/1.1\r\n" +
"Host: " + params.host + "\r\n" +
"Connection: close\r\n\r\n");
}
 return 0;
}

short pushDatamqtt(short field, float data, const Parameters & param, PubSubClient * client)
{
    String payload="field";
    payload+=String(field);
    payload+="=";
    payload+=String(data);
    payload+="&status=MQTTPUBLISH";

    if (client->connected()){
    Serial.print("Sending payload: ");
    Serial.println(payload);

    if (client->publish(param.topic, (char*) payload.c_str())) {
        Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  return 0;
}

void connectWiFi(Parameters params)
{
  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(params.ssid,params.password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);     
  } 
  Serial.println("\nConnected.");
}

void connectMQTTserver(Parameters params,PubSubClient mq_client)
{
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
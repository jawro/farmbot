#include "connections.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

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
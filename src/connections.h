#ifndef connections_h_
#define connections_h_
#include<PubSubClient.h>

struct Parameters
{
const char* ssid = "internety";
const char* password = "fffiiittt";

const char* mqtt_server = "mqtt.thingspeak.com";
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "8ABUGD4X994W7E06";

//char* topic="channels/<channelID/publish/<channelAPI>
char* topic = "channels/740255/publish/8ABUGD4X994W7E06"; 

const unsigned long ChannelID= 740255;

  Parameters(const char * ssid, const char * password, const char * host, const char * writeAPIKey);
  Parameters();
};

int pushData(short field, float data, const Parameters & param);

short pushDatamqtt(short field, float data, const Parameters & param,PubSubClient * client);

#endif

#ifndef connections_h_
#define connections_h_



struct Parameters
{
const char* ssid = "internety";
const char* password = "fffiiittt";

const char* host = "api.thingspeak.com";
const char* writeAPIKey = "8ABUGD4X994W7E06";

  Parameters(const char * ssid, const char * password, const char * host, const char * writeAPIKey);
  Parameters();
};
int pushData(short field, float data, const Parameters & param);


#endif

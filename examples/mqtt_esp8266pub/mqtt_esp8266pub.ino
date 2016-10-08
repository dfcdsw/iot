#include <ESP8266WiFi.h>
#include "Iot.h"


#define WLAN_SSID       "rd"
#define WLAN_PASS       "hidfrobot123"

#define AIO_SERVER      "183.230.40.39"
#define AIO_SERVERPORT  6002                   
#define AIO_USERNAME    "72833"
#define AIO_KEY         "pub123456"
#define AIO_CLIENTID    "3369568"

WiFiClient client;

const char MQTT_SERVER[]    = AIO_SERVER;
const char MQTT_USERNAME[]  = AIO_USERNAME;
const char MQTT_PASSWORD[]  = AIO_KEY;
const char MQTT_CLIENTID[]  = AIO_CLIENTID;

Iot iot(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println(F("DFrobot MQTT demo"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  
  iot.publish("temp");
}
int x=0;
void loop() {

  if(!iot.connected())Serial.printf("already connected!\n");
  else iot.connect(); 

  
  iot.writeInt("temp",x);
  iot.writeString("onoff","on");
  double test = 21.51;
  iot.writeDouble("change",test);
  x++;
  if(!iot.flush("temp"))Serial.printf("Pub OK!\n");
  delay(2000);
}



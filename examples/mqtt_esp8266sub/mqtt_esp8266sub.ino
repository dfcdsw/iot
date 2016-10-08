#include <ESP8266WiFi.h>
#include "Iot.h"


#define WLAN_SSID       "rd"
#define WLAN_PASS       "hidfrobot123"

#define AIO_SERVER      "183.230.40.39"
#define AIO_SERVERPORT  6002                   
#define AIO_USERNAME    "72833"
#define AIO_KEY         "sub123456"
#define AIO_CLIENTID    "3369569"

WiFiClient client;

const char MQTT_SERVER[]    = AIO_SERVER;
const char MQTT_USERNAME[]  = AIO_USERNAME;
const char MQTT_PASSWORD[]  = AIO_KEY;
const char MQTT_CLIENTID[]  = AIO_CLIENTID;




Iot iot(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

void subcallback(char *data, uint16_t len) {
  char recvmsg[100]={0};
  if(iot.parsejs(data))Serial.printf("parse ok\n");
  else return;
  if(iot.readAll(recvmsg,data,100))//If you do not know what msg about,use readAll to find msg
    Serial.printf("recvmsg = %s\n",recvmsg);

  int temp = iot.readInt("temp");Serial.printf("readInt temp = %d\n",temp);
  String onoff = iot.readstring("onoff");Serial.printf("readstring onoff = %s\n",onoff.c_str());
  double change = iot.readDouble("change");Serial.print("readDouble change =");Serial.println(change);
}

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
  
  if(iot.subscribe("temp"))Serial.printf("setSubscribe OK!\n");
  if(iot.subscribe("$creq"))Serial.printf("setSubscribe OK!\n");
  
  
  if(iot.setcallback("temp",subcallback))Serial.printf("set callbakc OK!\n");
 
}

void loop() {
    if(iot.connected())Serial.printf("already connected!\n");
    else iot.connect();  
    
    iot.available(1000);//You can ste timeout

    delay(300);
}



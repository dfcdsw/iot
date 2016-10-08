#ifndef _IOT_H_
#define _IOT_H_

#include "MQTT_Client.h"
#include "MQTT.h"
#include "cJSON.h"

#define SubTopicNum 4


class Iot 
{
public:
  Iot(Client *client, const char *server, uint16_t port,const char *cid, const char *user, const char *pass);
  ~Iot();
  bool subscribe(const char* subtopic);

  bool publish(const char* pubtopic);

  void connect();
  bool connected();

  void writeString(const char* dataID,const char* value);
  void writeInt(const char* dataID,int value);
  void writeDouble(const char* dataID,float value);

  bool flush(char* topicname);

  void available(int16_t timeout=500);
  bool setcallback(char* topicname,SubscribeCallbackBufferType callb);

  bool parsejs(char* data);
  bool readAll(char* recvmsg,char *data,int size);
  int readInt(char* key);
  String readstring(char* key);
  double readDouble(char* key);

private:
  char* pJSON;
  cJSON* parse;
  cJSON* root;
  
  int _size;
  
  char _subtopic1[20];
  char _subtopic2[20];
  char _subtopic3[20];
  char _subtopic4[20];
  char _pubtopic1[20];
  char _pubtopic2[20];
  char _pubtopic3[20];
  char _pubtopic4[20];
  MQTT_Client *Iotclient;
  MQTT_Subscribe *sensorSub1;
  MQTT_Subscribe *sensorSub2;
  MQTT_Subscribe *sensorSub3;
  MQTT_Subscribe *sensorSub4;

  
  MQTT_Publish *sensorPub1;
  MQTT_Publish *sensorPub2;
  MQTT_Publish *sensorPub3;
  MQTT_Publish *sensorPub4;
 
};

#endif

#include "Iot.h"
//#include <ArduinoJson.h>

int parseJson(char* recvmsg,cJSON * pJson, int iCnt);
void printntab(int iCnt);

Iot::Iot(Client *client, const char *server, uint16_t port,const char *cid, const char *user, const char *pass){
  
    Iotclient = new MQTT_Client(client, server, port, cid, user, pass);
    memset(_subtopic1,0,sizeof(_subtopic1));
    memset(_subtopic2,0,sizeof(_subtopic2));
    memset(_subtopic3,0,sizeof(_subtopic3));
    memset(_subtopic4,0,sizeof(_subtopic4));
    memset(_pubtopic1,0,sizeof(_pubtopic1));
    memset(_pubtopic2,0,sizeof(_pubtopic2));
    memset(_pubtopic3,0,sizeof(_pubtopic3));
    memset(_pubtopic4,0,sizeof(_pubtopic4));
    root = cJSON_CreateObject(); 
}

Iot::~Iot(){
    cJSON_Delete(root);
    pJSON = NULL;
    Iotclient->disconnectServer();
    free(Iotclient);
}



void Iot::writeString(const char* dataID,const char* value){
  cJSON_AddStringToObject(root,dataID,value);
}

void Iot::writeInt(const char* dataID,int value){
  cJSON_AddNumberToObject(root,dataID,value); 
}

void Iot::writeDouble(const char* dataID,float value){
  cJSON_AddNumberToObject(root,dataID,value);
}

int Iot::subscribe(const char* subtopic){
  if(strlen(_subtopic1) == 0){
    strcpy(_subtopic1,subtopic);
    sensorSub1 = new MQTT_Subscribe(Iotclient, _subtopic1,0); 
    Iotclient->subscribe(sensorSub1); 
  }else if(strlen(_subtopic2) == 0){
    strcpy(_subtopic2,subtopic);
    sensorSub2 = new MQTT_Subscribe(Iotclient, _subtopic2,0); 
    Iotclient->subscribe(sensorSub2);  
  }else if(strlen(_subtopic3) == 0){
    strcpy(_subtopic3,subtopic);
    sensorSub3 = new MQTT_Subscribe(Iotclient, _subtopic3,0);   
    Iotclient->subscribe(sensorSub3);
  }else if(strlen(_subtopic4) == 0){
    strcpy(_subtopic4,subtopic);
    sensorSub4 = new MQTT_Subscribe(Iotclient, _subtopic4,0);
    Iotclient->subscribe(sensorSub4);   
  }else{
    return -1;  
  }
  return 0; 
}

int Iot::publish(const char* pubtopic){
   if(strlen(_pubtopic1) == 0){
    strcpy(_pubtopic1,pubtopic);
    sensorPub1 = new MQTT_Publish(Iotclient, _pubtopic1,0); 
  }else if(strlen(_pubtopic2) == 0){
    strcpy(_pubtopic2,pubtopic);
    sensorPub2 = new MQTT_Publish(Iotclient, _pubtopic2,0);   
  }else if(strlen(_pubtopic3) == 0){
    strcpy(_pubtopic3,pubtopic);
    sensorPub3 = new MQTT_Publish(Iotclient, _pubtopic3,0);   
  }else if(strlen(_pubtopic4) == 0){
    strcpy(_pubtopic4,pubtopic);
    sensorPub4 = new MQTT_Publish(Iotclient, _pubtopic4,0);  
  }else{
    return -1;  
  }
  return 0;
}

int Iot::connected(){
  if (Iotclient->connected()) {
    return 0;
  }else{
    return -1;  
  }
}

void Iot::connect(){
  int8_t ret;

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 15;
  while ((ret = Iotclient->connect()) != 0) { 
       Serial.println(Iotclient->connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       Iotclient->disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

int Iot::flush(char* topicname){
  pJSON = cJSON_PrintUnformatted(root);
  if(pJSON){
    DBG("create js string is %s\n",pJSON);
    Serial.printf("pub msg is:%s\n",pJSON);
  }else{
    return -1;  
  }
  if(!strcmp(_pubtopic1,topicname)){
    if (! sensorPub1->publish(pJSON)) {
      //Serial.println(F("Failed"));
      cJSON_Delete(root);
      memset(pJSON,0,strlen(pJSON));
      root = cJSON_CreateObject();
      return -1;
    } else {
      //Serial.println(F("OK!"));
    }
  }else if(!strcmp(_pubtopic2,topicname)){
    if (! sensorPub2->publish(pJSON)) {
      //Serial.println(F("Failed"));
      cJSON_Delete(root);
      memset(pJSON,0,strlen(pJSON));
      root = cJSON_CreateObject();
      return -1;
    } else {
      //Serial.println(F("OK!"));
    }
  }else if(!strcmp(_pubtopic3,topicname)){
    if (! sensorPub3->publish(pJSON)) {
      //Serial.println(F("Failed"));
      cJSON_Delete(root);
      memset(pJSON,0,strlen(pJSON));
      root = cJSON_CreateObject();
      return -1;
    } else {
      //Serial.println(F("OK!"));
    }
  }else if(!strcmp(_pubtopic4,topicname)){
    if (! sensorPub4->publish(pJSON)) {
      //Serial.println(F("Failed"));
      cJSON_Delete(root);
      memset(pJSON,0,strlen(pJSON));
      root = cJSON_CreateObject();
      return -1;
    } else {
      //Serial.println(F("OK!"));
    }
  }else{
    cJSON_Delete(root);
    memset(pJSON,0,strlen(pJSON));
    root = cJSON_CreateObject();
    return -1;  
  }
   
  cJSON_Delete(root);
  memset(pJSON,0,strlen(pJSON));
  root = cJSON_CreateObject();
  return 0;
}

void Iot::available(int16_t timeout){
  Iotclient->processPackets(timeout);
}

int Iot::setcallback(char* topicname,SubscribeCallbackBufferType callb){
  if(!strcmp(_subtopic1,topicname)){
    sensorSub1->setCallback(callb);  
  }else if(!strcmp(_subtopic2,topicname)){
    sensorSub2->setCallback(callb);
  }else if(!strcmp(_subtopic3,topicname)){
    sensorSub3->setCallback(callb);
  }else if(!strcmp(_subtopic4,topicname)){
    sensorSub4->setCallback(callb);
  }else{
    return -1;  
  }
  return 0;
}

int Iot::parsejs(char* data){
  if(NULL != parse){
    cJSON_Delete(parse);  
  }
  parse = cJSON_Parse(data);
  if(NULL == parse)
  {
    DBG("parse err\n");
    return -1;
  }
  return 0;
}

int Iot::readAll(char* recvmsg,char *data){
  cJSON* parsing = cJSON_Parse(data);
    if(NULL == parsing)
    {
      DBG("parse err\n");
      return -1;
    }
    int iCnt = 0;
   parseJson(recvmsg,parsing, iCnt);
   cJSON_Delete(parsing);
   return 0;  
}

int Iot::readInt(char* key){
  if(!parse) {
    DBG("get readint faild !\n");
    return -1;
  }
  cJSON *temp = cJSON_GetObjectItem(parse, key);
  if(!temp) {
    DBG("No key !\n");
    return -1;
  }
  DBG("key type is %d\n",temp->type);
  DBG("key is %d\n",temp->valueint);
  int back=0;back = temp->valueint;
  return back;
}

String Iot::readstring(char* key){
  String err = "read err";
  if(!parse) {
    DBG("get readstring faild !\n");
    return err;
  }
  cJSON *temp = cJSON_GetObjectItem(parse, key);
  if(!temp) {
    DBG("No key !\n");
    return err;
  }
  //Serial.printf("key type is %d\n",temp->type);
  //Serial.printf("key is %s\n",temp->valuestring);
  DBG("key type is %d\n",temp->type);
  DBG("key is %d\n",temp->valueint);
  err = temp->valuestring;
  return err;
}

double Iot::readDouble(char* key){
  if(!parse) {
    DBG("get readdouble faild !\n");
    return -1;
  }
  cJSON *temp = cJSON_GetObjectItem(parse, key);
  if(!temp) {
    DBG("No key !\n");
    return -1;
  }
  DBG("key type is %d\n",temp->type);
  DBG("key is %lf\n",temp->valuedouble);
  float back = 0;back = temp->valuedouble;
  return back;
}

int parseJson(char* recvmsg,cJSON * parsing, int iCnt)
{
    if(NULL == parsing)
    {
        return -1;
    }
    switch(parsing->type)
    {
        case cJSON_False :
            {
                //Serial.printf("%s : %d\n", parsing->string, parsing->valueint);
            }
            break;
        case cJSON_True :
            {
                //Serial.printf("%s : %d\n", parsing->string, parsing->valueint);
            }
            break;
        case cJSON_NULL :
            {
                //Serial.printf("%s : NULL\n", parsing->string);
            }
            break;
        case cJSON_Number :
            {
                //Serial.printf("%s : %d | %lf\n", parsing->string, parsing->valueint, parsing->valuedouble);
                //Serial.printf("%s : %d\n", parsing->string, parsing->valueint);
                char str[20]={0};
                ftoa(str, parsing->valuedouble, 2); 
                sprintf(recvmsg,"%s:%d(int)|%s(double)\n",parsing->string, parsing->valueint, str);
                //Serial.printf("strlenint=%d,msg=%s\n",strlen(recvmsg),recvmsg);
            }
            break;
        case cJSON_String :
            {
                //Serial.printf("%s : %s\n", parsing->string, parsing->valuestring);
                sprintf(recvmsg,"%s:%s(string)\n",parsing->string, parsing->valuestring);
                //Serial.printf("strlenstring=%d,msg=%s\n",strlen(recvmsg),recvmsg);
            }
            break;
        case cJSON_Array  :
        case cJSON_Object :
            {
                int iSize = cJSON_GetArraySize(parsing);
                int i = 0;
                iCnt++;
                //Serial.printf("%s{\n", NULL == parsing->string ? "" : parsing->string);
                //if there is json 2 directory,should sprintf up a line
                for(i = 0; i < iSize; i++)
                {
                    printntab(iCnt);
                    cJSON * pSub = cJSON_GetArrayItem(parsing, i);
                    parseJson(recvmsg+strlen(recvmsg),pSub, iCnt);
                }
                printntab(iCnt);
                //Serial.printf("}\n");
            }
            break;
        default :
            DBG("\n");
            return -1;
            break;
    }
    DBG("\n");
}

void printntab(int iCnt)
{
    int i = 0;
    for(i = 0; i < iCnt; i++)
    {
        //Serial.printf("\t");
    }
}


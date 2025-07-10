#include "mqtt_Utils.h"

const int mqtt_port = 1883; //端口号
const char* mqtt_server = "120.26.64.89";   //ip地址
const char* mqtt_username = "zg";  //mqtt客户端的账号
const char* mqtt_password = "12345678";   //mqtt客户端的密码

WiFiClient espClient;
PubSubClient client(espClient);

//回调函数，用来接收指定订阅主题的信息
void callback(char* topic, byte* payload, unsigned int length) {

    String str = "";
    for(int i = 0; i < length; i++){
        str+= (char)payload[i];
    }
    //Serial.println(str);
    if(strcmp(topic,"qinaqiang") == 0){
        
        cJSON *root = cJSON_Parse(str.c_str());
        cJSON *boxId = cJSON_GetObjectItem(root,"boxId");

        char *str = cJSON_Print(boxId);

        if(*str > '0'){

            Serial.printf("柜门%s已打开\n",str);

        }else {

            Serial.printf("当前快递柜已满");
        }

    }

    

}

//连接函数
void reconnect() {
  // 循环到连接成功
  while (!client.connected()) {
    //随机指定一个客户端id
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    //真正的开始连接，这里我们的服务器规定了必须有账号密码
    if (client.connect(clientId.c_str(),mqtt_username,mqtt_password)) {
      Serial.println("connected");
      // 订阅一个指定主题
      client.subscribe("qinaqiang");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
    }
  }
}

//mqtt初始化
void mqtt_Init(){

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

//保持心跳，mqtt连接需要维持住，不能断
void mqtt_Loop(){

    if (!client.connected()) {
    reconnect();
    }
    client.loop();
}

void mqtt_sendData(String data){
     //随机指定一个客户端id
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

   if (client.connect(clientId.c_str(),mqtt_username,mqtt_password)) {
      
    //Serial.printf("data:%s\n",data.c_str());

    //创建一个cJSON
    cJSON *root = cJSON_CreateObject();

    cJSON_AddStringToObject(root,"boxName","qinaqiang");
    cJSON_AddStringToObject(root,"phone",data.c_str());

    char * strdata = cJSON_Print(root);
    //Serial.printf("data:%s\n",strdata);
      // 发送一个指定主题的信息
    client.publish("woniuSms", strdata);

    //释放内存
    cJSON_Delete(root);
    free(strdata);

    }   
  
  
}
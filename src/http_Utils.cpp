#include "http_Utils.h"

String http_Init(String url){

    HTTPClient http;    
        
    http.begin(url); //HTTP

    // 发送get请求
    int httpCode = http.GET();
    String payload;
    // 返回的状态值大于0，表示有响应
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            payload = http.getString();

        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return payload;

}

char * ParseID(String data){


    String url = "http://120.26.64.89:8080/box/getPackage/qinaqiang/" + data;
    
    String dataJSON = http_Init(url);

    cJSON *root  = cJSON_Parse(dataJSON.c_str());


    cJSON *boxIdJSON = cJSON_GetObjectItem(root,"boxId");


    char * boxId = cJSON_Print(boxIdJSON);
    return boxId;

}
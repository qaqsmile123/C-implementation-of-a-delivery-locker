#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H
#include <Arduino.h>
#include <HTTPClient.h>
#include "cJSON.h"
String http_Init(String url);
char* ParseID(String data);
#endif

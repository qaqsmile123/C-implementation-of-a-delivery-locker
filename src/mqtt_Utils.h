#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H
#include <Arduino.h>
#include <PubSubClient.h>
#include "WiFi_Control.h"
#include "cJSON.h"
void mqtt_Init();

void mqtt_Loop();

void mqtt_sendData(String data);

#endif
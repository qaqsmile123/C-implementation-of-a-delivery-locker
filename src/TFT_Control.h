#ifndef TFT_CONTROL_H
#define TFT_CONTROL_H
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "TAMC_GT911.h"

void Screen_Init();

void Screen_Loop();

#endif

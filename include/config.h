#pragma once
#include<Arduino.h>
#include<TFT_eSPI.h>

/*Actual screen size*/
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

/*Center calculation*/
constexpr int FACE_CENTER_X = SCREEN_WIDTH/2;
constexpr int FACE_CENTER_Y = SCREEN_HEIGHT/2;

/*Colors */
#define BG_COLOR TFT_BLACK
#define EYE_COLOR TFT_WHITE
#define IRIS_COLOR TFT_CYAN
#define PUPIL_COLOR TFT_BLACK
#define BLUSH_COLOR TFT_PINK
#define MOUTH_COLOR TFT_WHITE

/*BUTTON NAME SUBSTUTION */
#define BUTTON_A WIO_KEY_A
#define BUTTON_B WIO_KEY_B
#define BUTTON_C WIO_KEY_C

/*Buzzer declaration*/
#define BUZZER_PIN WIO_BUZZER

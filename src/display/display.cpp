#include "display.h"
#include "../../include/config.h"

TFT_eSPI tft = TFT_eSPI();

void initDisplay(){
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(BG_COLOR);
}
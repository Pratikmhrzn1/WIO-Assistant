#include "display.h"

#include "config.h"

TFT_eSPI tft = TFT_eSPI();

void init_display()
{
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(BG_COLOR);
}

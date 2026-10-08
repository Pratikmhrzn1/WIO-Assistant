#include "buzzer.h"
#include <Arduino.h>
#include "../../include/config.h"

void initBuzzer(){
    pinMode(BUZZER_PIN,OUTPUT);
    digitalWrite(BUZZER_PIN,LOW);
}

void dizzyBeep(){
    tone(BUZZER_PIN,1000,150);
    delay(100);
    tone(BUZZER_PIN,700,150);
    delay(100);
    noTone(BUZZER_PIN);
}
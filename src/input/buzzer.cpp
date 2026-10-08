#include "buzzer.h"

#include <Arduino.h>

#include "config.h"

// =====================================================
// INITIALIZE BUZZER
// =====================================================

void init_buzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

// =====================================================
// DIZZY BEEP
// =====================================================

void dizzy_beep()
{
    tone(BUZZER_PIN, 1000, 150);
    delay(100);
    tone(BUZZER_PIN, 700, 150);
    delay(100);
    noTone(BUZZER_PIN);
}

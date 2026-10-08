#include <Arduino.h>

#include "config.h"

#include "display/display.h"
#include "face/face.h"
#include "animations/animation.h"
#include "input/button.h"
#include "input/accelerometer.h"
#include "input/buzzer.h"

// =====================================================
// CURRENT EXPRESSION
// =====================================================

face_expression_t current_expression = NORMAL;

// =====================================================
// SETUP
// =====================================================

void setup()
{
    Serial.begin(115200);

    // Display
    init_display();

    // Buttons
    init_buttons();

    // Accelerometer
    init_accelerometer();

    // Buzzer
    init_buzzer();
    current_expression = NORMAL;

    // Initial face
    draw_normal_face();

    Serial.println("Wio Terminal Face Ready!");
    Serial.println("A = Happy");
    Serial.println("B = Sleepy");
    Serial.println("C = Surprised");
    Serial.println("Shake = Dizzy");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
    // =================================================
    // SHAKE
    // =================================================

    if (shake_detected())
    {
        Serial.println("Shake detected!");

        current_expression = DIZZY;
        dizzy_beep();
        dizzy_animation();

        current_expression = NORMAL;

        return;
    }

    // =================================================
    // BUTTON A
    // HAPPY
    // =================================================

    if (button_pressed(BUTTON_A))
    {
        Serial.println("Happy reaction!");

        current_expression = HAPPY;
        happy_animation();
        current_expression = NORMAL;
    }

    // =================================================
    // BUTTON B
    // SLEEPY
    // =================================================

    if (button_pressed(BUTTON_B))
    {
        Serial.println("Sleepy reaction!");

        current_expression = SLEEPY;
        sleepy_animation();
        current_expression = NORMAL;
    }

    // =================================================
    // BUTTON C
    // SURPRISED
    // =================================================

    if (button_pressed(BUTTON_C))
    {
        Serial.println("Surprised reaction!");

        current_expression = SURPRISED;
        surprised_animation();
        current_expression = NORMAL;
    }

    // =================================================
    // IDLE
    // =================================================

    delay(10);
}

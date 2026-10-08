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

FaceExpression currentExpression = NORMAL;


// =====================================================
// SETUP
// =====================================================

void setup()
{
    Serial.begin(115200);

    // Display
    initDisplay();

    // Buttons
    initButtons();

    // Accelerometer
    initAccelerometer();

    //Buzzer
    initBuzzer();
    currentExpression = NORMAL;

    // Initial face
    drawNormalFace();

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

    if (shakeDetected())
    {
        Serial.println("Shake detected!");

        currentExpression = DIZZY;
        dizzyBeep();
        dizzyAnimation();

        currentExpression = NORMAL;

        return;
    }


    // =================================================
    // BUTTON A
    // HAPPY
    // =================================================

    if (buttonPressed(BUTTON_A))
    {
        Serial.println("Happy reaction!");

        currentExpression = HAPPY;

        happyAnimation();

        currentExpression = NORMAL;
    }


    // =================================================
    // BUTTON B
    // SLEEPY
    // =================================================

    if (buttonPressed(BUTTON_B))
    {
        Serial.println("Sleepy reaction!");

        currentExpression = SLEEPY;
        
        sleepyAnimation();

        currentExpression = NORMAL;
    }


    // =================================================
    // BUTTON C
    // SURPRISED
    // =================================================

    if (buttonPressed(BUTTON_C))
    {
        Serial.println("Surprised reaction!");

        currentExpression = SURPRISED;

        surprisedAnimation();

        currentExpression = NORMAL;
    }


    // =================================================
    // IDLE
    // =================================================

    delay(10);
}
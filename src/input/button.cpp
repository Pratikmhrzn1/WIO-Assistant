#include "button.h"

#include "config.h"

// =====================================================
// INITIALIZE BUTTONS
// =====================================================

void initButtons()
{
    pinMode(
        BUTTON_A,
        INPUT_PULLUP
    );

    pinMode(
        BUTTON_B,
        INPUT_PULLUP
    );

    pinMode(
        BUTTON_C,
        INPUT_PULLUP
    );
}


// =====================================================
// BUTTON PRESSED
// =====================================================

bool buttonPressed(int button)
{
    // Button pressed
    if (digitalRead(button) == LOW)
    {
        // Debounce
        delay(30);

        // Check again
        if (digitalRead(button) == LOW)
        {
            // Wait until released
            while (digitalRead(button) == LOW)
            {
                delay(5);
            }

            return true;
        }
    }

    return false;
}
#include "accelerometer.h"

#include <Arduino.h>
#include <Wire.h>
#include <LIS3DHTR.h>

// =====================================================
// ACCELEROMETER
// =====================================================

LIS3DHTR<TwoWire> lis;

// =====================================================
// SHAKE SETTINGS
// =====================================================

// Increase this if the device triggers too easily.
constexpr float SHAKE_THRESHOLD = 2.5f;

// Minimum time between shake detections.
constexpr unsigned long SHAKE_COOLDOWN = 800;

unsigned long last_shake_time = 0;

// =====================================================
// INITIALIZE ACCELEROMETER
// =====================================================

void init_accelerometer()
{
    lis.begin(Wire1);

    delay(100);

    lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
    lis.setFullScaleRange(LIS3DHTR_RANGE_2G);
}

// =====================================================
// SHAKE DETECTION
// =====================================================

bool shake_detected()
{
    if (!lis.available())
    {
        return false;
    }

    // Read acceleration

    float x = lis.getAccelerationX();
    float y = lis.getAccelerationY();
    float z = lis.getAccelerationZ();

    // Calculate total acceleration magnitude

    float magnitude = sqrt(x * x + y * y + z * z);

    // Check cooldown

    if (millis() - last_shake_time < SHAKE_COOLDOWN)
    {
        return false;
    }

    // Detect strong movement

    if (magnitude > SHAKE_THRESHOLD)
    {
        last_shake_time = millis();

        return true;
    }

    return false;
}

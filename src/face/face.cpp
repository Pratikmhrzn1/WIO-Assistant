#include "face.h"

#include "config.h"
#include "display/display.h"

// =====================================================
// DRAW LEFT EYE
// =====================================================

void drawLeftEye(
    int x,
    int y,
    int eyeSize,
    int irisSize,
    int pupilSize
)
{
    // -------------------------------------------------
    // White eye
    // -------------------------------------------------

    tft.fillCircle(
        x,
        y,
        eyeSize,
        EYE_COLOR
    );

    // -------------------------------------------------
    // Iris
    // -------------------------------------------------

    tft.fillCircle(
        x,
        y + 3,
        irisSize,
        IRIS_COLOR
    );

    // -------------------------------------------------
    // Pupil
    // -------------------------------------------------

    tft.fillCircle(
        x,
        y + 5,
        pupilSize,
        PUPIL_COLOR
    );

    // -------------------------------------------------
    // Eye highlights
    // -------------------------------------------------

    tft.fillCircle(
        x - 7,
        y - 8,
        6,
        TFT_WHITE
    );

    tft.fillCircle(
        x + 7,
        y - 2,
        3,
        TFT_WHITE
    );
}


// =====================================================
// DRAW RIGHT EYE
// =====================================================

void drawRightEye(
    int x,
    int y,
    int eyeSize,
    int irisSize,
    int pupilSize
)
{
    // -------------------------------------------------
    // White eye
    // -------------------------------------------------

    tft.fillCircle(
        x,
        y,
        eyeSize,
        EYE_COLOR
    );

    // -------------------------------------------------
    // Iris
    // -------------------------------------------------

    tft.fillCircle(
        x,
        y + 3,
        irisSize,
        IRIS_COLOR
    );

    // -------------------------------------------------
    // Pupil
    // -------------------------------------------------

    tft.fillCircle(
        x,
        y + 5,
        pupilSize,
        PUPIL_COLOR
    );

    // -------------------------------------------------
    // Eye highlights
    // -------------------------------------------------

    tft.fillCircle(
        x - 7,
        y - 8,
        6,
        TFT_WHITE
    );

    tft.fillCircle(
        x + 7,
        y - 2,
        3,
        TFT_WHITE
    );
}


// =====================================================
// DRAW NORMAL EYES
// =====================================================

void drawNormalEyes(
    int offsetX,
    int offsetY
)
{
    drawLeftEye(
        100 + offsetX,
        110 + offsetY,
        38,
        23,
        13
    );

    drawRightEye(
        220 + offsetX,
        110 + offsetY,
        38,
        23,
        13
    );
}


// =====================================================
// DRAW CHEEKS
// =====================================================

void drawCheeks()
{
    // Left cheek
    tft.fillCircle(
        55,
        165,
        13,
        BLUSH_COLOR
    );

    // Right cheek
    tft.fillCircle(
        265,
        165,
        13,
        BLUSH_COLOR
    );
}


// =====================================================
// NORMAL MOUTH
// =====================================================

void drawNormalMouth()
{
    tft.drawLine(
        150,
        175,
        160,
        180,
        MOUTH_COLOR
    );

    tft.drawLine(
        160,
        180,
        170,
        175,
        MOUTH_COLOR
    );
}


// =====================================================
// HAPPY MOUTH
// =====================================================

void drawHappyMouth()
{
    // Bigger smile

    tft.drawLine(
        140,
        170,
        150,
        180,
        MOUTH_COLOR
    );

    tft.drawLine(
        150,
        180,
        160,
        183,
        MOUTH_COLOR
    );

    tft.drawLine(
        160,
        183,
        170,
        180,
        MOUTH_COLOR
    );

    tft.drawLine(
        170,
        180,
        180,
        170,
        MOUTH_COLOR
    );
}


// =====================================================
// SLEEPY MOUTH
// =====================================================

void drawSleepyMouth()
{
    tft.drawLine(
        150,
        180,
        170,
        180,
        MOUTH_COLOR
    );
}


// =====================================================
// SURPRISED MOUTH
// =====================================================

void drawSurprisedMouth()
{
    // Outer mouth

    tft.fillCircle(
        160,
        180,
        10,
        MOUTH_COLOR
    );

    // Inner mouth

    tft.fillCircle(
        160,
        180,
        6,
        BG_COLOR
    );
}


// =====================================================
// NORMAL EYEBROWS
// =====================================================

void drawNormalEyebrows()
{
    // Left eyebrow

    tft.drawLine(
        75,
        60,
        125,
        55,
        TFT_WHITE
    );

    // Right eyebrow

    tft.drawLine(
        195,
        55,
        245,
        60,
        TFT_WHITE
    );
}


// =====================================================
// HAPPY EYEBROWS
// =====================================================

void drawHappyEyebrows()
{
    // Left eyebrow

    tft.drawLine(
        75,
        58,
        125,
        50,
        TFT_WHITE
    );

    // Right eyebrow

    tft.drawLine(
        195,
        50,
        245,
        58,
        TFT_WHITE
    );
}


// =====================================================
// SLEEPY EYEBROWS
// =====================================================

void drawSleepyEyebrows()
{
    // Left eyebrow

    tft.drawLine(
        75,
        65,
        125,
        70,
        TFT_WHITE
    );

    // Right eyebrow

    tft.drawLine(
        195,
        70,
        245,
        65,
        TFT_WHITE
    );
}


// =====================================================
// SURPRISED EYEBROWS
// =====================================================

void drawSurprisedEyebrows()
{
    // Left eyebrow

    tft.drawLine(
        70,
        45,
        125,
        50,
        TFT_WHITE
    );

    // Right eyebrow

    tft.drawLine(
        195,
        50,
        250,
        45,
        TFT_WHITE
    );
}


// =====================================================
// NORMAL FACE
// =====================================================

void drawNormalFace(
    int offsetX,
    int offsetY
)
{
    tft.fillScreen(BG_COLOR);

    drawNormalEyebrows();

    drawNormalEyes(
        offsetX,
        offsetY
    );

    drawCheeks();

    drawNormalMouth();
}


// =====================================================
// HAPPY FACE
// =====================================================

void drawHappyFace(
    int offsetX,
    int offsetY
)
{
    tft.fillScreen(BG_COLOR);

    drawHappyEyebrows();

    // Slightly larger eyes

    drawLeftEye(
        100 + offsetX,
        108 + offsetY,
        40,
        25,
        14
    );

    drawRightEye(
        220 + offsetX,
        108 + offsetY,
        40,
        25,
        14
    );

    drawCheeks();

    drawHappyMouth();
}


// =====================================================
// SLEEPY FACE
// =====================================================

void drawSleepyFace()
{
    tft.fillScreen(BG_COLOR);

    drawSleepyEyebrows();

    // -------------------------------------------------
    // Closed left eye
    // -------------------------------------------------

    tft.drawLine(
        70,
        110,
        130,
        115,
        EYE_COLOR
    );

    // -------------------------------------------------
    // Closed right eye
    // -------------------------------------------------

    tft.drawLine(
        190,
        115,
        250,
        110,
        EYE_COLOR
    );

    // -------------------------------------------------
    // Small eyelashes
    // -------------------------------------------------

    tft.drawLine(
        75,
        112,
        70,
        118,
        EYE_COLOR
    );

    tft.drawLine(
        245,
        112,
        250,
        118,
        EYE_COLOR
    );

    drawCheeks();

    drawSleepyMouth();
}


// =====================================================
// SURPRISED FACE
// =====================================================

void drawSurprisedFace(
    int offsetY
)
{
    tft.fillScreen(BG_COLOR);

    drawSurprisedEyebrows();

    // -------------------------------------------------
    // Large left eye
    // -------------------------------------------------

    drawLeftEye(
        100,
        110 + offsetY,
        43,
        27,
        15
    );

    // -------------------------------------------------
    // Large right eye
    // -------------------------------------------------

    drawRightEye(
        220,
        110 + offsetY,
        43,
        27,
        15
    );

    drawCheeks();

    drawSurprisedMouth();
}

// =====================================================
// DIZZY EYEBROWS
// =====================================================

void drawDizzyEyebrows()
{
    // Left eyebrow - tilted

    tft.drawLine(
        75,
        55,
        125,
        65,
        TFT_WHITE
    );

    // Right eyebrow - tilted opposite direction

    tft.drawLine(
        195,
        65,
        245,
        55,
        TFT_WHITE
    );
}

// =====================================================
// DIZZY MOUTH
// =====================================================

void drawDizzyMouth()
{
    // Wobbly mouth

    tft.drawLine(
        145,
        175,
        152,
        180,
        MOUTH_COLOR
    );

    tft.drawLine(
        152,
        180,
        160,
        174,
        MOUTH_COLOR
    );

    tft.drawLine(
        160,
        174,
        168,
        180,
        MOUTH_COLOR
    );

    tft.drawLine(
        168,
        180,
        175,
        175,
        MOUTH_COLOR
    );
}

// =====================================================
// DIZZY FACE
// =====================================================

void drawDizzyFace(
    int offsetX,
    int offsetY
)
{
    tft.fillScreen(BG_COLOR);

    drawDizzyEyebrows();

    // -------------------------------------------------
    // Left dizzy eye
    // -------------------------------------------------

    int leftX = 100 + offsetX;
    int leftY = 110 + offsetY;

    tft.drawCircle(
        leftX,
        leftY,
        32,
        EYE_COLOR
    );

    // X-shaped eye

    tft.drawLine(
        leftX - 15,
        leftY - 15,
        leftX + 15,
        leftY + 15,
        EYE_COLOR
    );

    tft.drawLine(
        leftX + 15,
        leftY - 15,
        leftX - 15,
        leftY + 15,
        EYE_COLOR
    );


    // -------------------------------------------------
    // Right dizzy eye
    // -------------------------------------------------

    int rightX = 220 + offsetX;
    int rightY = 110 + offsetY;

    tft.drawCircle(
        rightX,
        rightY,
        32,
        EYE_COLOR
    );

    // X-shaped eye

    tft.drawLine(
        rightX - 15,
        rightY - 15,
        rightX + 15,
        rightY + 15,
        EYE_COLOR
    );

    tft.drawLine(
        rightX + 15,
        rightY - 15,
        rightX - 15,
        rightY + 15,
        EYE_COLOR
    );

    // -------------------------------------------------
    // Cheeks
    // -------------------------------------------------

    drawCheeks();

    // -------------------------------------------------
    // Dizzy mouth
    // -------------------------------------------------

    drawDizzyMouth();
}
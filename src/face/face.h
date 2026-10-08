#pragma once

// =====================================================
// FACE EXPRESSIONS
// =====================================================

enum FaceExpression
{
    NORMAL,
    HAPPY,
    SLEEPY,
    SURPRISED,
    DIZZY
};

// =====================================================
// EYES
// =====================================================

void drawLeftEye(
    int x,
    int y,
    int eyeSize,
    int irisSize,
    int pupilSize
);

void drawRightEye(
    int x,
    int y,
    int eyeSize,
    int irisSize,
    int pupilSize
);

void drawNormalEyes(
    int offsetX = 0,
    int offsetY = 0
);

// =====================================================
// CHEEKS
// =====================================================

void drawCheeks();

// =====================================================
// MOUTHS
// =====================================================

void drawNormalMouth();
void drawHappyMouth();
void drawSleepyMouth();
void drawSurprisedMouth();
void drawDizzyMouth();

// =====================================================
// EYEBROWS
// =====================================================

void drawNormalEyebrows();
void drawHappyEyebrows();
void drawSleepyEyebrows();
void drawSurprisedEyebrows();
void drawDizzyEyebrows();

// =====================================================
// COMPLETE FACES
// =====================================================

void drawNormalFace(
    int offsetX = 0,
    int offsetY = 0
);

void drawHappyFace(
    int offsetX = 0,
    int offsetY = 0
);

void drawSleepyFace();

void drawSurprisedFace(
    int offsetY = 0
);

void drawDizzyFace(
    int offsetX = 0,
    int offsetY = 0
);
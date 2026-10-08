#pragma once

// =====================================================
// FACE EXPRESSIONS
// =====================================================

enum face_expression_t
{
    NORMAL,
    HAPPY,
    SLEEPY,
    SURPRISED,
    DIZZY
};

// =====================================================
// COMPLETE FACES
// =====================================================

void draw_normal_face(int offset_x = 0, int offset_y = 0);

void draw_happy_face(int offset_x = 0, int offset_y = 0);

void draw_surprised_face(int offset_y = 0);

void draw_dizzy_face(int offset_x = 0, int offset_y = 0);

// =====================================================
// PARTIAL FRAMES (used by the animations)
// =====================================================

void draw_blink_frame();

void draw_sleepy_frame(int lid_y);

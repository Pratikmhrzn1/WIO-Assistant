#pragma once

#include "config.h"
#include "primitives.h"

// =====================================================
// EYE STYLE
// =====================================================

enum eye_style_t : uint8_t
{
    EYE_FILLED, // white sclera + iris + pupil + highlights
    EYE_CLOSED, // a single horizontal lid line
    EYE_X_MARK  // hollow ring + X (dizzy)
};

// =====================================================
// FACE SPEC
// =====================================================
//
// Everything needed to draw one expression. The five
// expressions below are just rows of this struct — the
// renderer in face.cpp knows nothing about individual
// faces, it only walks these tables.
//

struct face_spec_t
{
    const prim_t *brows;
    uint8_t brow_count;

    const prim_t *mouths;
    uint8_t mouth_count;

    eye_style_t style;

    int16_t eye_y;

    uint8_t eye_r;    // EYE_FILLED
    uint8_t iris_r;   // EYE_FILLED
    uint8_t pupil_r;  // EYE_FILLED
    uint8_t socket_r; // EYE_X_MARK
    int8_t lid_half_w; // EYE_CLOSED
};

// =====================================================
// EYEBROWS
// =====================================================

constexpr prim_t BROWS_NORMAL[] = {
    prim_line(75, 60, 125, 55, TFT_WHITE),
    prim_line(195, 55, 245, 60, TFT_WHITE),
};

constexpr prim_t BROWS_HAPPY[] = {
    prim_line(75, 58, 125, 50, TFT_WHITE),
    prim_line(195, 50, 245, 58, TFT_WHITE),
};

constexpr prim_t BROWS_SLEEPY[] = {
    prim_line(75, 65, 125, 70, TFT_WHITE),
    prim_line(195, 70, 245, 65, TFT_WHITE),
};

constexpr prim_t BROWS_SURPRISED[] = {
    prim_line(70, 45, 125, 50, TFT_WHITE),
    prim_line(195, 50, 250, 45, TFT_WHITE),
};

constexpr prim_t BROWS_DIZZY[] = {
    prim_line(75, 55, 125, 65, TFT_WHITE),
    prim_line(195, 65, 245, 55, TFT_WHITE),
};

// =====================================================
// MOUTHS
// =====================================================

constexpr prim_t MOUTH_NORMAL[] = {
    prim_line(150, 175, 160, 180, MOUTH_COLOR),
    prim_line(160, 180, 170, 175, MOUTH_COLOR),
};

constexpr prim_t MOUTH_HAPPY[] = {
    prim_line(140, 170, 150, 180, MOUTH_COLOR),
    prim_line(150, 180, 160, 183, MOUTH_COLOR),
    prim_line(160, 183, 170, 180, MOUTH_COLOR),
    prim_line(170, 180, 180, 170, MOUTH_COLOR),
};

constexpr prim_t MOUTH_SLEEPY[] = {
    prim_line(150, 180, 170, 180, MOUTH_COLOR),
};

// dark ring with the background punched out = an open "O"
constexpr prim_t MOUTH_SURPRISED[] = {
    prim_disc(160, 180, 10, MOUTH_COLOR),
    prim_disc(160, 180, 6, BG_COLOR),
};

constexpr prim_t MOUTH_DIZZY[] = {
    prim_line(145, 175, 152, 180, MOUTH_COLOR),
    prim_line(152, 180, 160, 174, MOUTH_COLOR),
    prim_line(160, 174, 168, 180, MOUTH_COLOR),
    prim_line(168, 180, 175, 175, MOUTH_COLOR),
};

// =====================================================
// FACE SPECS
// =====================================================

// brows, mouth, style, eye_y, eye_r, iris_r, pupil_r, socket_r, lid_half_w

constexpr face_spec_t SPEC_NORMAL = {
    BROWS_NORMAL, 2, MOUTH_NORMAL, 2,
    EYE_FILLED, 110, 38, 23, 13, 0, 0};

constexpr face_spec_t SPEC_HAPPY = {
    BROWS_HAPPY, 2, MOUTH_HAPPY, 4,
    EYE_FILLED, 108, 40, 25, 14, 0, 0};

constexpr face_spec_t SPEC_SLEEPY = {
    BROWS_SLEEPY, 2, MOUTH_SLEEPY, 1,
    EYE_CLOSED, 108, 0, 0, 0, 0, 30};

constexpr face_spec_t SPEC_SURPRISED = {
    BROWS_SURPRISED, 2, MOUTH_SURPRISED, 2,
    EYE_FILLED, 110, 43, 27, 15, 0, 0};

constexpr face_spec_t SPEC_DIZZY = {
    BROWS_DIZZY, 2, MOUTH_DIZZY, 4,
    EYE_X_MARK, 110, 0, 0, 0, 32, 0};

// normal brows + mouth with the lids shut — used by blink
constexpr face_spec_t SPEC_BLINK = {
    BROWS_NORMAL, 2, MOUTH_NORMAL, 2,
    EYE_CLOSED, 110, 0, 0, 0, 0, 35};

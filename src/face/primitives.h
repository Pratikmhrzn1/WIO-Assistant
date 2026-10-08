#pragma once

#include <stdint.h>

// =====================================================
// PRIMITIVE KINDS
// =====================================================

enum prim_kind_t : uint8_t
{
    PRIM_LINE,        // x0,y0 -> x1,y1
    PRIM_FILL_CIRCLE, // x0,y0 = centre, x1 = radius
    PRIM_RING         // x0,y0 = centre, x1 = radius (outline only)
};

// =====================================================
// PRIMITIVE
// =====================================================
//
// One drawable shape. Coordinates are raw screen pixels
// (no offset applied yet) so the same value can be reused
// by every expression and shifted at draw time.
//

struct prim_t
{
    uint8_t kind;
    uint16_t color;
    int16_t x0, y0;
    int16_t x1, y1; // circles: x1 = radius, y1 unused
};

// =====================================================
// CONSTEXPR BUILDERS (used by the face data tables)
// =====================================================

constexpr prim_t prim_line(
    int16_t x0, int16_t y0,
    int16_t x1, int16_t y1,
    uint16_t color)
{
    return {PRIM_LINE, color, x0, y0, x1, y1};
}

constexpr prim_t prim_disc(
    int16_t x, int16_t y,
    int16_t r,
    uint16_t color)
{
    return {PRIM_FILL_CIRCLE, color, x, y, r, 0};
}

// =====================================================
// DRAWING
// =====================================================

void draw_prim(const prim_t &p, int offset_x = 0, int offset_y = 0);

void draw_eye(
    int x, int y,
    int radius,
    int iris_radius,
    int pupil_radius);

void draw_x_eye(int x, int y, int radius);

void draw_closed_eye(int center_x, int y, int half_width);

void draw_cheeks(int offset_x = 0, int offset_y = 0);

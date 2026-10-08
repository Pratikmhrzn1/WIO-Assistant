#include "primitives.h"

#include "config.h"
#include "display/display.h"

// =====================================================
// DRAW ONE PRIMITIVE
// =====================================================

void draw_prim(const prim_t &p, int offset_x, int offset_y)
{
    switch (p.kind)
    {
    case PRIM_LINE:
        tft.drawLine(
            p.x0 + offset_x, p.y0 + offset_y,
            p.x1 + offset_x, p.y1 + offset_y,
            p.color);
        break;

    case PRIM_FILL_CIRCLE:
        tft.fillCircle(
            p.x0 + offset_x, p.y0 + offset_y,
            p.x1,
            p.color);
        break;

    case PRIM_RING:
        tft.drawCircle(
            p.x0 + offset_x, p.y0 + offset_y,
            p.x1,
            p.color);
        break;
    }
}

// =====================================================
// DRAW ONE EYE
// =====================================================
//
// Layered back to front: sclera -> iris -> pupil ->
// two highlights. The iris and pupil sit slightly below
// centre so the eye appears to be looking at you, and
// the asymmetric white dots read as a light reflection.
//

void draw_eye(
    int x, int y,
    int radius,
    int iris_radius,
    int pupil_radius)
{
    tft.fillCircle(x, y, radius, EYE_COLOR);
    tft.fillCircle(x, y + 3, iris_radius, IRIS_COLOR);
    tft.fillCircle(x, y + 5, pupil_radius, PUPIL_COLOR);

    tft.fillCircle(x - 7, y - 8, 6, TFT_WHITE);
    tft.fillCircle(x + 7, y - 2, 3, TFT_WHITE);
}

// =====================================================
// DRAW ONE DIZZY EYE
// =====================================================
//
// Hollow socket ring plus an X — the classic cartoon
// "knocked out" symbol.
//

void draw_x_eye(int x, int y, int radius)
{
    tft.drawCircle(x, y, radius, EYE_COLOR);

    tft.drawLine(
        x - 15, y - 15,
        x + 15, y + 15,
        EYE_COLOR);

    tft.drawLine(
        x + 15, y - 15,
        x - 15, y + 15,
        EYE_COLOR);
}

// =====================================================
// DRAW ONE CLOSED EYE
// =====================================================

void draw_closed_eye(int center_x, int y, int half_width)
{
    tft.drawLine(
        center_x - half_width, y,
        center_x + half_width, y,
        EYE_COLOR);
}

// =====================================================
// DRAW CHEEKS
// =====================================================

void draw_cheeks(int offset_x, int offset_y)
{
    tft.fillCircle(55 + offset_x, 165 + offset_y, 13, BLUSH_COLOR);
    tft.fillCircle(265 + offset_x, 165 + offset_y, 13, BLUSH_COLOR);
}

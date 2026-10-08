#include "face.h"

#include "config.h"
#include "display/display.h"
#include "face_data.h"
#include "primitives.h"

// =====================================================
// EYE COLUMN POSITIONS
// =====================================================

static constexpr int LEFT_EYE_X = 100;
static constexpr int RIGHT_EYE_X = 220;

// =====================================================
// GENERIC FACE RENDERER
// =====================================================
//
// Painter's algorithm: clear -> brows -> eyes -> cheeks
// -> mouth. Every expression in face_data.h is drawn by
// this one function; the spec decides which shapes land
// on screen and where.
//

static void render_face(
    const face_spec_t &spec,
    int offset_x = 0,
    int offset_y = 0)
{
    tft.fillScreen(BG_COLOR);

    // -------------------------------------------------
    // Eyebrows
    // -------------------------------------------------

    for (uint8_t i = 0; i < spec.brow_count; i++)
    {
        draw_prim(spec.brows[i], offset_x, offset_y);
    }

    // -------------------------------------------------
    // Eyes
    // -------------------------------------------------

    int y = spec.eye_y + offset_y;
    int left_x = LEFT_EYE_X + offset_x;
    int right_x = RIGHT_EYE_X + offset_x;

    switch (spec.style)
    {
    case EYE_FILLED:
        draw_eye(left_x, y, spec.eye_r, spec.iris_r, spec.pupil_r);
        draw_eye(right_x, y, spec.eye_r, spec.iris_r, spec.pupil_r);
        break;

    case EYE_CLOSED:
        draw_closed_eye(left_x, y, spec.lid_half_w);
        draw_closed_eye(right_x, y, spec.lid_half_w);
        break;

    case EYE_X_MARK:
        draw_x_eye(left_x, y, spec.socket_r);
        draw_x_eye(right_x, y, spec.socket_r);
        break;
    }

    // -------------------------------------------------
    // Cheeks
    // -------------------------------------------------

    draw_cheeks(offset_x, offset_y);

    // -------------------------------------------------
    // Mouth
    // -------------------------------------------------

    for (uint8_t i = 0; i < spec.mouth_count; i++)
    {
        draw_prim(spec.mouths[i], offset_x, offset_y);
    }
}

// =====================================================
// COMPLETE FACES
// =====================================================

void draw_normal_face(int offset_x, int offset_y)
{
    render_face(SPEC_NORMAL, offset_x, offset_y);
}

void draw_happy_face(int offset_x, int offset_y)
{
    render_face(SPEC_HAPPY, offset_x, offset_y);
}

void draw_surprised_face(int offset_y)
{
    render_face(SPEC_SURPRISED, 0, offset_y);
}

void draw_dizzy_face(int offset_x, int offset_y)
{
    render_face(SPEC_DIZZY, offset_x, offset_y);
}

// =====================================================
// PARTIAL FRAMES
// =====================================================

void draw_blink_frame()
{
    render_face(SPEC_BLINK);
}

// Same sleepy expression, but with the lid line moved to
// a caller-chosen height so the animation can lower it
// one step at a time.
void draw_sleepy_frame(int lid_y)
{
    face_spec_t spec = SPEC_SLEEPY;
    spec.eye_y = lid_y;

    render_face(spec);
}

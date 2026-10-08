# WioFaceChanger

A face-changing emoji bot for the **Seeed Wio Terminal**. It draws a cartoon face on the
320×240 LCD and switches expressions when you press a button or shake the device.

| Trigger | Expression | Feedback |
|---|---|---|
| Button A | 😊 Happy | hop animation + blink |
| Button B | 😴 Sleepy | eyelids slowly close, then wake up |
| Button C | 😲 Surprised | eyes pop bigger |
| Shake | 😵 Dizzy | face wobbles + buzzer beep |
| (idle) | 🙂 Normal | default resting face |

---

## Table of Contents

1. [Hardware & Project Layout](#1-hardware--project-layout)
2. [Step-by-Step Build Guide](#2-step-by-step-build-guide)
3. [How the Code Is Organized](#3-how-the-code-is-organized)
4. [Library-by-Library Explainer](#4-library-by-library-explainer)
5. [The Display Side](#5-the-display-side)
6. [The Animation Side](#6-the-animation-side)
7. [Input: Buttons, Accelerometer, Buzzer](#7-input-buttons-accelerometer-buzzer)
8. [The Main Loop](#8-the-main-loop)
9. [Configuration Reference (`config.h`)](#9/configuration-reference-configh)
10. [Adding Your Own Expression](#10-adding-your-own-expression)
11. [Troubleshooting](#11-troubleshooting)

---

## 1. Hardware & Project Layout

**Board:** Seeed Wio Terminal — an all-in-one dev board containing:

- SAMD21 ARM Cortex-M0+ MCU @ 128 MHz
- 320×240 2.4" IPS TFT LCD (driven by an ST7789-class controller)
- LIS3DHTR 3-axis accelerometer (onboard, on `Wire1`)
- 3 user buttons (`A`, `B`, `C`) on the front face
- Built-in buzzer
- microSD slot, speaker, light sensor, IR emitter (unused here)

**Directory structure:**

```
WioFaceChanger/
├── platformio.ini          ← build config + library dependencies
├── include/
│   └── config.h            ← screen size, colors, pin aliases
├── src/
│   ├── main.cpp            ← setup() + loop() — the orchestrator
│   ├── display/
│   │   ├── display.h       ← declares the shared `tft` object
│   │   └── display.cpp     ← creates + initializes `tft`
│   ├── face/
│   │   ├── face.h          ← FaceExpression enum + draw prototypes
│   │   └── face.cpp        ← all shape-drawing code (726 lines)
│   ├── animations/
│   │   ├── animation.h     ← 5 animation prototypes
│   │   └── animation.cpp   ← keyframe sequences built from face draws
│   └── input/
│       ├── button.h/.cpp       ← debounced button reading
│       ├── accelerometer.h/.cpp← shake detection
│       └── buzzer.h/.cpp       ← beep sounds
├── lib/                    ← for *your* private libraries (empty)
└── test/                   ← for unit tests (empty)
```

**Design rule of thumb:** dependencies only point *downward*.

```
main.cpp  →  animations  →  face  →  display  →  TFT_eSPI
   ↓            ↓           ↓
 input/      config.h    config.h
```

Nothing in `face/` knows about buttons. Nothing in `display/` knows about faces.

---

## 2. Step-by-Step Build Guide

### Step 0 — Prerequisites

- [PlatformIO](https://platformio.org/) installed (either the VS Code extension or the
  `pio` CLI).
- A USB-C cable and a Wio Terminal.

Check it works:

```bash
pio --version
```

### Step 1 — Create / open the project

Open this folder in VS Code with the PlatformIO extension, or use the CLI:

```bash
pio init --board seeed_wio_terminal
```

### Step 2 — Understand `platformio.ini`

This one file is the entire build system:

```ini
[env:seeed_wio_terminal]      ; a named build environment
platform = atmelsam            ; Atmel SAM toolchain (GCC ARM + bossac uploader)
board = seeed_wio_terminal     ; pin map, flash size, upload protocol for the Wio
framework = arduino            ; use the Arduino C++ core instead of bare-metal HAL

lib_deps =                     ; libraries auto-downloaded into .pio/libdeps/
    Seeed_Arduino_LCD          ; provides TFT_eSPI (the LCD driver)
    https://github.com/Seeed-Studio/Seeed_Arduino_LIS3DHTR.git   ; accelerometer driver
```

**Every line explained:**

| Line | What it means |
|---|---|
| `[env:...]` | Defines a build target. You can stack several envs for different boards. |
| `platform = atmelsam` | Downloads the ARM-GCC compiler, OpenOCD/bossac tools, and SAMD21 support. |
| `board = seeed_wio_terminal` | Tells PlatformIO the MCU is a SAMD21G18A, 256 KB flash, 32 KB RAM, USB CDC upload. |
| `framework = arduino` | Pulls in `Arduino.h`, `setup()`, `loop()`, `millis()`, `digitalRead()`, `tone()`, `Wire`… |
| `lib_deps` | Declared dependencies. On first build PlatformIO clones them into `.pio/libdeps/seeed_wio_terminal/`. |

### Step 3 — First build

```bash
pio run
```

What happens, in order:

1. PlatformIO reads `platformio.ini`.
2. Downloads missing toolchain + libraries (first run only, ~1–2 min).
3. Compiles every `.cpp` under `src/` (and each library's sources) into `.o` files in
   `.pio/build/seeed_wio_terminal/`.
4. Links them into `firmware.elf`, then converts to `firmware.bin`.
5. Reports flash/RAM usage.

### Step 4 — Upload to the board

```bash
pio run --target upload
```

Hold the *reset* button while plugging in if the upload doesn't start (this puts the
Wio into its bootloader).

### Step 5 — Watch the serial log

```bash
pio device monitor -b 115200
```

You should see:

```
Wio Terminal Face Ready!
A = Happy
B = Sleepy
C = Surprised
Shake = Dizzy
```

Then press A/B/C or shake the device.

> **Tip:** `pio run -t upload -t monitor` does build + flash + serial in one shot.

### Step 6 — Iterate

Edit a file → `pio run -t upload` → observe. PlatformIO only recompiles what changed.

---

## 3. How the Code Is Organized

`main.cpp` is deliberately dumb. It is a pure **event dispatcher**:

```
setup():
    Serial → Display → Buttons → Accelerometer → Buzzer → draw normal face

loop():
    shake detected?  → DIZZY  → dizzyBeep()   → dizzyAnimation()     → back to NORMAL
    button A?        → HAPPY                   → happyAnimation()     → back to NORMAL
    button B?        → SLEEPY                  → sleepyAnimation()    → back to NORMAL
    button C?        → SURPRISED               → surprisedAnimation() → back to NORMAL
    else             → delay(10)   (idle tick)
```

The global `FaceExpression currentExpression` tracks what's on screen. It's set before
each animation and reset to `NORMAL` after — animations are self-contained
"reactions" that always end where they started.

Each subsystem exposes an **init function** called once in `setup()` and a small
**verb API** used in `loop()`:

| Module | Init | API |
|---|---|---|
| display | `initDisplay()` | `tft` (global) |
| buttons | `initButtons()` | `buttonPressed(pin)` |
| accelerometer | `initAccelerometer()` | `shakeDetected()` |
| buzzer | `initBuzzer()` | `dizzyBeep()` |
| face | — | `drawNormalFace()`, `drawHappyFace()`, … |
| animation | — | `happyAnimation()`, `blinkAnimation()`, … |

---

## 4. Library-by-Library Explainer

### 4.1 `TFT_eSPI` (from `Seeed_Arduino_LCD`)

A fast, popular Arduino graphics library for SPI TFT displays. The Wio Terminal ships
with a Seeed-maintained fork pre-configured for its built-in screen, so you don't have
to edit any pin config.

**Every method this project uses:**

| Method | Signature | What it does |
|---|---|---|
| `begin()` | `tft.begin()` | Powers up the LCD, sends init commands over SPI. Must be called first. |
| `setRotation()` | `tft.setRotation(3)` | Rotates the coordinate system. `3` = landscape with the USB port on the left. Screen becomes 320 wide × 240 tall. |
| `fillScreen()` | `tft.fillScreen(TFT_BLACK)` | Floods the **entire** display with one color. Fastest way to clear. |
| `fillCircle()` | `tft.fillCircle(x, y, radius, color)` | Solid filled circle. Used for eyes, irises, pupils, cheeks, highlights, the surprised mouth. |
| `drawCircle()` | `tft.drawCircle(x, y, radius, color)` | Outline-only circle. Used for the dizzy eye sockets. |
| `drawLine()` | `tft.drawLine(x0, y0, x1, y1, color)` | Straight line. Used for eyebrows, mouths, closed eyelids, and the `X` eyes. |

**Predefined color constants** (from the library, no need to define your own):
`TFT_BLACK`, `TFT_WHITE`, `TFT_CYAN`, `TFT_PINK`, `TFT_RED`, `TFT_GREEN`, `TFT_BLUE`,
`TFT_YELLOW`, `TFT_ORANGE`, `TFT_GREY`, …

Colors are 16-bit RGB565: 5 bits red, 6 bits green, 5 bits blue — one `uint16_t`
per pixel, which is why the framebuffer is only 320×240×2 = 150 KB.

**Key concept — immediate mode:** `TFT_eSPI` keeps *no* record of what you drew.
Call `fillCircle()` and those pixels are permanently changed until something else
overwrites them. That's why every face draw starts with `fillScreen()`: it's the
only way to "erase" the previous frame.

### 4.2 `LIS3DHTR` (Seeed's accelerometer driver)

A C++ driver for the ST LIS3DH 3-axis accelerometer over I²C.

**What this project uses:**

| Call | Meaning |
|---|---|
| `LIS3DHTR<TwoWire> lis;` | Template parameter = the I²C bus class. `TwoWire` is standard Arduino I²C (`Wire`). |
| `lis.begin(Wire1)` | Attach the sensor to bus `Wire1`. **Important:** the Wio Terminal's onboard accelerometer lives on `Wire1`, not the default `Wire` (which is on the Grove ports). |
| `lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ)` | Sample 25 times/second. Low rate = less I²C traffic, plenty for shake detection. |
| `lis.setFullScaleRange(LIS3DHTR_RANGE_2G)` | ±2 g range → highest resolution for hand motions. |
| `lis.available()` | `true` once the sensor has responded and is ready to read. |
| `lis.getAccelerationX/Y/Z()` | Current acceleration in **g** units (float). At rest, one axis reads ≈ ±1 g (gravity), the others ≈ 0. |

**Concepts:**

- **I²C (Inter-Integrated Circuit):** a 2-wire bus (SDA/SCL) where the MCU is the
  *master* and sensors are *addresses*. `Wire`/`Wire1` are Arduino's bus objects.
- **Data rate vs. resolution:** 25 Hz means new samples every 40 ms. `loop()` polls
  far faster than that, so consecutive reads often return the same value — harmless.
- **Full-scale range:** ±2 g means the reported value clips at ±2.0. For a shake
  threshold of 2.5 g *magnitude* (see §7.2), the combined vector of all three axes is
  what exceeds the limit, not a single axis.

### 4.3 Arduino core functions used

| Function | Where | Purpose |
|---|---|---|
| `Serial.begin(115200)` | `setup()` | Start USB serial at 115200 baud for debug prints. |
| `pinMode(pin, INPUT_PULLUP)` | `initButtons()` | Configure a pin as input with the internal pull-up resistor enabled → pin idles `HIGH`, reads `LOW` when the button connects it to ground. |
| `digitalRead(pin)` | `buttonPressed()` | Read a digital pin → `HIGH` or `LOW`. |
| `pinMode(pin, OUTPUT)` / `digitalWrite(pin, LOW)` | `initBuzzer()` | Ensure the buzzer starts silent. |
| `tone(pin, freq, duration)` | `dizzyBeep()` | Generate a square wave of `freq` Hz on `pin` for `duration` ms — makes the piezo buzz. |
| `noTone(pin)` | `dizzyBeep()` | Stop the wave. |
| `delay(ms)` | everywhere | **Blocking** pause. Nothing else runs during it — this is what paces the animations. |
| `millis()` | `shakeDetected()` | Milliseconds since boot, free-running. Used for non-blocking timing (the cooldown). |
| `sqrt(x*x + y*y + z*z)` | `shakeDetected()` | Vector magnitude of the 3-axis acceleration. |

### 4.4 C++ language features used

| Feature | Example | Why it's used |
|---|---|---|
| `#pragma once` | top of every `.h` | Include-guard — prevents double-definition when a header is included from multiple files. |
| `extern` | `extern TFT_eSPI tft;` in `display.h` | "This variable exists, defined in *another* file." Lets every module share one LCD object without duplicate-definition linker errors. |
| `constexpr int` | `SCREEN_WIDTH = 320` in `config.h` | Compile-time constant, typed (unlike `#define`), doesn't pollute the preprocessor. |
| `#define` alias | `#define BUTTON_A WIO_KEY_A` | Short, readable names for board pin macros. |
| `enum FaceExpression` | `face.h` | A named set of values (`NORMAL`=0, `HAPPY`=1, …). Makes `currentExpression` self-documenting. |
| **Default arguments** | `void drawNormalFace(int offsetX = 0, int offsetY = 0)` | Callers can omit the offsets: `drawNormalFace()` still compiles. |
| `.cpp` / `.h` split | every module | Header = contract (what exists). Source = implementation (how it works). Keeps compile times low and prevents duplicate symbol errors. |

---

## 5. The Display Side

Three files, and the whole job of the layer is to own **one shared object**.

```cpp
// display.h
#pragma once
#include <TFT_eSPI.h>
extern TFT_eSPI tft;      // "declared elsewhere — just let me use it"
void initDisplay();

// display.cpp
TFT_eSPI tft = TFT_eSPI(); // ← THE one instance, constructed here

void initDisplay() {
    tft.begin();                  // 1. wake the LCD controller
    tft.setRotation(3);           // 2. landscape, origin top-left, 320×240
    tft.fillScreen(BG_COLOR);     // 3. clear to black
}
```

**Why `extern` matters:** if each `.cpp` file wrote `TFT_eSPI tft;`, the linker would
find five definitions of the same symbol and fail. `display.cpp` *defines* it once;
everyone else *declares* it. The `#include "config.h"` in `display.cpp` pulls in
`BG_COLOR`.

**Rotation explained:** `setRotation(0..3)` cycles through 0°/90°/180°/270°. On the
Wio Terminal, `3` is the natural landscape orientation (buttons on the right, USB on
the left), giving `SCREEN_WIDTH = 320` and `SCREEN_HEIGHT = 240`.

**The coordinate system** after rotation 3:

```
(0,0) ──────────────────────────► x (0..319)
  │
  │      eyebrows ~ y 45–70
  │      eyes      ~ y 110 (center y = 120)
  │      cheeks    ~ y 165
  │      mouth     ~ y 175–183
  ▼
y (0..239)
```

The face is drawn slightly *above* vertical center (eyes at y≈110 vs. center 120),
which leaves comfortable room for the mouth and reads as a "head" rather than a
centered diagram.

---

## 6. The Animation Side

This is the part worth studying, because it uses a technique worth naming.

### 6.1 There is no animation engine

No frame buffer of poses, no easing curves, no delta-time, no sprite system. An
animation is simply:

> **draw a face at an offset → `delay()` → draw it at another offset → `delay()` → …**

The gaps between draws are your brain's job: it perceives continuous motion from a
handful of discrete keyframes. This is exactly how classic flipbook / cel animation
works, and on a 320×240 screen it's more than convincing.

### 6.2 The offset trick — pseudo-translation

Every face function accepts `offsetX`/`offsetY` (default `0`), and adds them to every
coordinate:

```cpp
void drawNormalEyes(int offsetX, int offsetY) {
    drawLeftEye(100 + offsetX, 110 + offsetY, 38, 23, 13);
    drawRightEye(220 + offsetX, 110 + offsetY, 38, 23, 13);
}
```

Because the whole face is redrawn anyway (via `fillScreen()` first), shifting the
arguments **is** the translation. You never move pixels — you just draw the next
frame somewhere else. This also means there's no ghosting/smearing: full clear +
redraw each keyframe.

### 6.3 The layering order (painter's algorithm)

Every face draw happens back-to-front, so later shapes occlude earlier ones:

```
1. fillScreen(BG_COLOR)     ← erase everything
2. eyebrows                 ← farthest back, nothing covers them
3. eyes                     ← sclera, then iris, then pupil, then highlights
4. cheeks
5. mouth                    ← drawn last, on top
```

Within an eye, the stacking is what makes it look like an eye:

```
fillCircle(eyeSize)         white sclera
  fillCircle(irisSize)      cyan iris, offset +3px down
    fillCircle(pupilSize)   black pupil, offset +5px down
      fillCircle(6) white   big specular highlight, up-left
      fillCircle(3) white   small highlight, right
```

The downward offsets (`+3`, `+5`) simulate a slight "looking at you" gaze; the two
asymmetric white dots simulate light reflection and are what give the eye life.

### 6.4 Each animation, keyframe by keyframe

**`blinkAnimation()`** — a *partial* redraw. It does **not** call a face function;
it manually clears, draws brows, and replaces each eye with a single horizontal
`drawLine` at y=110 (a closed lid). 100 ms closed, then `drawNormalFace()` restores
everything. Total ≈ 200 ms — roughly the duration of a real blink.

**`happyAnimation()`** — a hop:

```
frame  drawHappyFace(0,  0)   hold 150 ms
frame  drawHappyFace(0, -5)   hold 100 ms   ← lifts up
frame  drawHappyFace(0, +5)   hold 100 ms   ← drops down (overshoot)
frame  drawHappyFace(0,  0)   hold 200 ms   ← settles
       blinkAnimation()                     ← adds life
       drawNormalFace()                     ← return to idle
```

Note it *overshoots* downward before settling — that's a crude but effective
bounce/anticipation curve. Also note `drawHappyFace` uses bigger eyes
(r 40/25/14 vs. 38/23/13) and higher-arched brows for the expression itself.

**`sleepyAnimation()`** — the one animation that uses a **loop** instead of literal
keyframes:

```cpp
for (int i = 0; i < 3; i++) {
    tft.fillScreen(BG_COLOR);
    drawSleepyEyebrows();
    int y = 108 + (i * 3);      // eyelid line descends 3px per frame
    tft.drawLine(70, y, 130, y, EYE_COLOR);   // left lid
    tft.drawLine(190, y, 250, y, EYE_COLOR);  // right lid
    drawCheeks();
    drawSleepyMouth();
    delay(120);
}
delay(500);          // stay asleep
// wake up: 3 × normal face at 100 ms — a quick flicker back to alert
```

Gradual descent (3 frames × 3 px) reads as *slowly closing*, which is the whole
point of "sleepy" — vs. the instant 100 ms snap of a blink.

**`surprisedAnimation()`** — vertical bounce of the eye sizes via `offsetY`:

```
drawNormalFace()          100 ms   baseline
drawSurprisedFace(-3)     120 ms   eyes shrink slightly (anticipation)
drawSurprisedFace(0)      120 ms   normal surprised size
drawSurprisedFace(+3)     120 ms   eyes bulge — the "pop"
drawSurprisedFace(0)      500 ms   hold the reaction
drawNormalFace()                   recover
```

The surprised face itself uses the largest eyes in the app (43/27/15), raised brows
(y 45 vs. y 60), and a round "O" mouth built from two `fillCircle`s (dark ring =
outer mouth color, background-colored inner circle = open hole).

**`dizzyAnimation()`** — the widest, fastest movement:

```
(0, 0)   100 ms
(-8, 0)   80 ms   ← hard left
(+8, 0)   80 ms   ← hard right
(-6, +3)  80 ms   ← left + dip
(+6, -3)  80 ms   ← right + rise
(0, 0)   300 ms   ← settle, still dazed
normal    150 ms   ← recover
```

Short 80 ms holds + large ±8 px offsets = jittery. The alternating vertical tilt
(`+3`/`-3`) breaks the purely horizontal motion so it looks disoriented rather than
sliding. The face itself is unique: **outline** circles + two crossing lines per eye
(an `X`, the classic "knocked out" cartoon symbol) instead of filled eyes, tilted
opposing brows, and a zig-zag wobbly mouth built from 4 short segments.

### 6.5 Timing cheat-sheet

| Animation | Frames | Total duration | Motion character |
|---|---|---|---|
| blink | 2 | ~200 ms | instant snap |
| happy | 4 + blink | ~750 ms | bounce / hop |
| sleepy | 3 + hold + 3 | ~1.6 s | slow, gradual |
| surprised | 4 + hold | ~980 ms | anticipation → pop |
| dizzy | 5 + hold | ~940 ms | fast, erratic |

### 6.6 Known trade-offs (and why they're fine here)

- **`delay()` is blocking.** During an animation, buttons and shake are ignored.
  For a toy face that's desirable — reactions are atomic events. If you needed
  responsiveness you'd refactor to a state machine driven by `millis()`.
- **Full `fillScreen()` each frame.** Wasteful (you're erasing pixels you'll redraw),
  but simple and 100% free of artifacts. On this display a full clear is a few ms.
- **No easing functions.** Keyframes are evenly timed. Adding ease-in/out would mean
  computing interpolated offsets — a nice upgrade described in §10.

---

## 7. Input: Buttons, Accelerometer, Buzzer

### 7.1 Buttons (`input/button.cpp`)

```cpp
void initButtons() {
    pinMode(BUTTON_A, INPUT_PULLUP);   // and B, C
}
```

**How a Wio button works:** the pin is tied to 3.3 V through a *pull-up* resistor and
the button shorts it to ground. So `digitalRead()` returns `HIGH` when idle and `LOW`
when pressed. `INPUT_PULLUP` enables the MCU's internal resistor so you don't need an
external one.

**`buttonPressed()` — three-stage debounce + edge detection:**

```cpp
if (digitalRead(button) == LOW) {      // 1. looks pressed?
    delay(30);                          // 2. wait 30 ms for contact bounce to settle
    if (digitalRead(button) == LOW) {   // 3. still pressed? then it's real
        while (digitalRead(button) == LOW) delay(5);   // 4. block until release
        return true;                                  // 5. fire exactly ONCE
    }
}
return false;
```

- **Contact bounce:** mechanical contacts physically vibrate for a few ms when they
  close, producing a burst of rapid HIGH/LOW transitions. Without debounce you'd get
  3–5 "presses" per physical press. The 30 ms wait rides it out.
- **Wait-for-release:** this converts *level* (held) into a single *event* (pressed).
  Without it, holding button A would re-trigger `happyAnimation()` hundreds of times
  per second.

The trade-off: this function blocks while held, and it's called sequentially in
`loop()` — so only one button can be handled per pass, and A wins over B over C.
Perfectly acceptable for this device.

### 7.2 Accelerometer / shake (`input/accelerometer.cpp`)

```cpp
constexpr float SHAKE_THRESHOLD   = 2.5f;   // g — total acceleration magnitude
constexpr unsigned long SHAKE_COOLDOWN = 800; // ms between accepted shakes
unsigned long lastShakeTime = 0;
```

```cpp
bool shakeDetected() {
    if (!lis.available()) return false;

    float x = lis.getAccelerationX(), y = ..., z = ...;
    float magnitude = sqrt(x*x + y*y + z*z);   // vector length

    if (millis() - lastShakeTime < SHAKE_COOLDOWN) return false;  // rate limit

    if (magnitude > SHAKE_THRESHOLD) {
        lastShakeTime = millis();   // reset the timer
        return true;
    }
    return false;
}
```

**Why magnitude instead of one axis?** A shake changes direction constantly; at some
moments the motion is along X, sometimes Y, sometimes diagonal. The Euclidean
magnitude `√(x²+y²+z²)` is direction-independent. At rest it equals ~1.0 g (gravity).
A vigorous shake spikes it well above 2.5 g.

**Why a cooldown?** A single shake produces many consecutive above-threshold samples
(25 Hz sampling over ~300 ms ⇒ ~7 readings). Without the 800 ms lockout, one shake
would fire `dizzyAnimation()` repeatedly and the device would appear stuck. `millis()`
gives non-blocking, wraparound-safe timing: `millis() - last` is correct even when
`millis()` rolls over at ~49.7 days.

**Tuning:** raise `SHAKE_THRESHOLD` if it triggers from normal handling; lower it if
you have to shake hard. Raise `SHAKE_COOLDOWN` to make it less trigger-happy.

### 7.3 Buzzer (`input/buzzer.cpp`)

```cpp
void initBuzzer() { pinMode(BUZZER_PIN, OUTPUT); digitalWrite(BUZZER_PIN, LOW); }

void dizzyBeep() {
    tone(BUZZER_PIN, 1000, 150);  // 1000 Hz for 150 ms
    delay(100);
    tone(BUZZER_PIN, 700, 150);   // 700 Hz for 150 ms — descending "uh-oh"
    delay(100);
    noTone(BUZZER_PIN);
}
```

A two-note descending motif (high → low) is a near-universal audio cue for
"something's wrong / I'm dizzy" — perfectly matching the expression. `tone()`
generates a square wave on a PWM pin; the piezo converts it to sound.

---

## 8. The Main Loop

```cpp
void loop() {
    if (shakeDetected()) {          // checked FIRST — most dramatic reaction
        currentExpression = DIZZY;
        dizzyBeep();                // sound plays while the animation runs
        dizzyAnimation();
        currentExpression = NORMAL;
        return;                     // skip the rest of this pass
    }

    if (buttonPressed(BUTTON_A)) { ... HAPPY ... }   // blocking until released
    if (buttonPressed(BUTTON_B)) { ... SLEEPY ... }
    if (buttonPressed(BUTTON_C)) { ... SURPRISED ... }

    delay(10);                      // idle tick — ~100 loop passes/sec
}
```

Reading order:

1. **Shake first** so a vigorous shake isn't mistaken for button activity, and
   `return` gives it exclusive ownership of the pass.
2. **Buttons are polled**, not interrupt-driven. `buttonPressed()` blocks during
   debounce and until release, so there's no way to double-fire.
3. **`delay(10)`** prevents the loop from spinning at 100% CPU. It costs nothing
   meaningful for responsiveness since everything else is already blocking.
4. Each handler follows the same three-line ritual: *set expression → animate →
   reset to NORMAL*. Because the reset is unconditional, the device always returns
   to a known state, even if an animation is later modified to bail out early.

**Serial output** (`Serial.println(...)`) in each branch is pure debug telemetry —
open the monitor and you can see which event fired without looking at the screen.

---

## 9. Configuration Reference (`config.h`)

```cpp
constexpr int SCREEN_WIDTH  = 320;   // after setRotation(3)
constexpr int SCREEN_HEIGHT = 240;
constexpr int FACE_CENTER_X = SCREEN_WIDTH / 2;   // 160
constexpr int FACE_CENTER_Y = SCREEN_HEIGHT / 2;  // 120

#define BG_COLOR     TFT_BLACK   // screen background
#define EYE_COLOR    TFT_WHITE   // sclera, eyelids, dizzy X-eyes
#define IRIS_COLOR   TFT_CYAN    // the colorful part of the eye
#define PUPIL_COLOR  TFT_BLACK   // center dot
#define BLUSH_COLOR  TFT_PINK    // cheeks
#define MOUTH_COLOR  TFT_WHITE   // all mouth lines

#define BUTTON_A WIO_KEY_A       // board pin macros, aliased for readability
#define BUTTON_B WIO_KEY_B
#define BUTTON_C WIO_KEY_C

#define BUZZER_PIN WIO_BUZZER    // onboard piezo pin
```

**To reskin the whole app**, edit these six color macros — no drawing code changes
needed. `constexpr` (rather than `#define`) for the sizes means real types, scoping,
and better compiler diagnostics; `#define` is kept for colors/pins because
`TFT_WHITE`, `WIO_KEY_A`, etc. are themselves macros and must be substituted
textually.

`FACE_CENTER_X/Y` are currently defined but unused — the drawing code hard-codes
coordinates. See §10 for how you could put them to work.

---

## 10. Adding Your Own Expression

Say you want a **angry** face on a new button. Five touches:

**1. Enum** — `src/face/face.h`:

```cpp
enum FaceExpression { NORMAL, HAPPY, SLEEPY, SURPRISED, DIZZY, ANGRY };
```

**2. Parts** — `src/face/face.cpp` (+ declare in `face.h`):

```cpp
void drawAngryEyebrows() {          // steep downward slant toward the nose
    tft.drawLine(75, 50, 125, 68, TFT_WHITE);
    tft.drawLine(195, 68, 245, 50, TFT_WHITE);
}
void drawAngryMouth() {             // inverted arc = frown
    tft.drawLine(140, 183, 155, 172, MOUTH_COLOR);
    tft.drawLine(155, 172, 175, 183, MOUTH_COLOR);
}
void drawAngryFace(int offsetX = 0, int offsetY = 0) {
    tft.fillScreen(BG_COLOR);
    drawAngryEyebrows();
    drawLeftEye(100 + offsetX, 110 + offsetY, 36, 22, 12);   // slightly narrowed
    drawRightEye(220 + offsetX, 110 + offsetY, 36, 22, 12);
    drawCheeks();
    drawAngryMouth();
}
```

**3. Animation** — `src/animations/animation.cpp` (+ `.h`):

```cpp
void angryAnimation() {
    drawAngryFace(-4, 0);  delay(90);    // lunge left
    drawAngryFace( 4, 0);  delay(90);    // lunge right
    drawAngryFace(-4, 0);  delay(90);
    drawAngryFace( 4, 0);  delay(90);
    drawAngryFace( 0, 0);  delay(400);   // seethe
    drawNormalFace();
}
```

**4. Wire a trigger** — `src/main.cpp`, in `loop()`:

```cpp
if (buttonPressed(BUTTON_D)) {          // + init pinMode in initButtons()
    currentExpression = ANGRY;
    angryAnimation();
    currentExpression = NORMAL;
}
```

**5. Rebuild:** `pio run -t upload`.

**Bigger refactors worth considering:**

- **Table-driven keyframes.** Replace hand-written sequences with
  `{faceFn, offsetX, offsetY, ms}` arrays and one generic `playKeyframes()` runner.
  Adds a function-pointer type — a good first step toward a real animation system.
- **`millis()`-based timing.** Store `nextFrameAt` and check it in `loop()` instead
  of calling `delay()`. Animations become non-blocking, so buttons/shake stay live
  mid-animation.
- **Use `FACE_CENTER_X/Y`.** Derive eye/mouth positions from the center constant
  instead of magic numbers, making the face resolution-independent.
- **Partial redraws.** Only clear the bounding box of what changes rather than
  `fillScreen()`, for smoother frames.

---

## 11. Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| Upload fails / port not found | Board not in bootloader | Hold **reset** while plugging in USB, then re-run upload. |
| Black screen after upload | Wrong rotation or LCD not initialized | Confirm `initDisplay()` runs before any draw; check `setRotation(3)`. |
| `TFT_eSPI` not found | Library missing | `pio run` once to fetch `lib_deps`, or `pio pkg install`. |
| `Wire1` / accelerometer errors | Using default `Wire` | The onboard LIS3DH is on `Wire1` — `lis.begin(Wire1)` is required. |
| Shake triggers constantly | Threshold too low | Increase `SHAKE_THRESHOLD` (e.g. `3.0f`) in `accelerometer.cpp`. |
| Shake never triggers | Threshold too high, or sensor not ready | Lower it; also check `lis.available()` and the `delay(100)` after `begin()`. |
| Button fires multiple times | Debounce too short | Increase `delay(30)` in `buttonPressed()`. |
| One button press does nothing | Another button/shake handled first | Each `if` is sequential; only one fires per pass by design. |
| Animation feels laggy | Holds too long | Shorten the `delay()` values in `animation.cpp`. |
| Serial monitor garbled | Baud mismatch | Use `-b 115200` to match `Serial.begin(115200)`. |

**Useful commands:**

```bash
pio run                     # build only
pio run -t upload           # build + flash
pio run -t clean            # delete build artifacts
pio device monitor -b 115200  # serial output
pio device list             # show connected serial ports
```

---

## Quick Reference: Every Drawing Call in the Project

| Call | Count of uses | Purpose |
|---|---|---|
| `tft.fillScreen(color)` | 1 per face/frame | Clear entire display |
| `tft.fillCircle(x,y,r,color)` | ~30 | Eyes, irises, pupils, highlights, cheeks, round mouths |
| `tft.drawCircle(x,y,r,color)` | 2 | Dizzy eye outlines (hollow) |
| `tft.drawLine(x0,y0,x1,y1,color)` | ~40 | Eyebrows, mouths, eyelids, X-eyes |

That's the entire graphics vocabulary — five functions producing every face and
every animation in the app.

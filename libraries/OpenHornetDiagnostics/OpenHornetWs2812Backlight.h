/**************************************************************************************
 *        ____                   _    _                       _
 *       / __ \                 | |  | |                     | |
 *      | |  | |_ __   ___ _ __ | |__| | ___  _ __ _ __   ___| |_
 *      | |  | | '_ \ / _ \ '_ \|  __  |/ _ \| '__| '_ \ / _ \ __|
 *      | |__| | |_) |  __/ | | | |  | | (_) | |  | | | |  __/ |_
 *       \____/| .__/ \___|_| |_|_|  |_|\___/|_|  |_| |_|\___|\__|
 *             | |
 *             |_|
 *   ----------------------------------------------------------------------------------
 *   Copyright 2016-2026 OpenHornet
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *   ----------------------------------------------------------------------------------
 *   Note: All other portions of OpenHornet not within the 'OpenHornet-Software'
 *   GitHub repository is released under the Creative Commons Attribution -
 *   Non-Commercial - Share Alike License. (CC BY-NC-SA 4.0)
 *   ----------------------------------------------------------------------------------
 *   This Project uses Doxygen as a documentation generator.
 *   Please use Doxygen capable comments.
 **************************************************************************************/

/**
 * @file OpenHornetWs2812Backlight.h
 * @author lahirunirmalx
 * @date 10.14.2026
 * @version 0.3.0
 * @copyright Copyright 2016-2026 OpenHornet. Licensed under the Apache License, Version 2.0.
 * @brief WS2812 backlight writer for OhBacklightDiagnostics (DDI/AMPCD boards with addressable backlighting).
 *
 * @details
 * Turns the 0-255 level computed by OhBacklightDiagnostics into a solid colour on a
 * string of WS2812 pixels with FastLED. Include it only on a board whose backlight is a
 * WS2812 string; the released DDI/AMPCD boards use plain LEDs on the PWM pin and do not
 * need it.
 *
 * The data pin, pixel count and colour order are board specific and are NOT known for
 * any published OpenHornet DDI/AMPCD revision. Define them in the sketch before the
 * include to match the board in hand:
 *
 * @code
 * #define OH_WS2812_PIN 9          // data pin of the backlight string
 * #define OH_WS2812_COUNT 20       // number of pixels in the string
 * #define OH_WS2812_ORDER GRB      // wire order of the pixels (FastLED EOrder)
 * #define OH_DIAG_MIN_WRITE_MS 20  // see OpenHornetDiagnostics.h
 * #include "OpenHornetDiagnostics.h"
 * #include "OpenHornetWs2812Backlight.h"
 *
 * OhBacklightDiagnostics backlight(ohWs2812BacklightWrite);
 *
 * void setup() {
 *   ohWs2812BacklightBegin();  // before backlight.begin()
 *   backlight.begin();
 * }
 * @endcode
 *
 * FastLED.show() keeps interrupts off for about 30 us per pixel on AVR, so keep
 * OH_DIAG_MIN_WRITE_MS around 20 on this path to leave room for DCS-BIOS serial.
 */

#ifndef OPENHORNET_WS2812_BACKLIGHT_H
#define OPENHORNET_WS2812_BACKLIGHT_H

#include "Arduino.h"
#include <FastLED.h>

#ifndef OH_WS2812_PIN
#error "OH_WS2812_PIN (backlight data pin) must be defined before including OpenHornetWs2812Backlight.h"
#endif
#ifndef OH_WS2812_COUNT
#error "OH_WS2812_COUNT (number of backlight pixels) must be defined before including OpenHornetWs2812Backlight.h"
#endif
#ifndef OH_WS2812_ORDER
#define OH_WS2812_ORDER GRB  ///< Pixel colour order. GRB is the common WS2812B wiring (as on 4A3A3).
#endif
#ifndef OH_WS2812_COLOR
#define OH_WS2812_COLOR CRGB(25, 155, 0)  ///< Full brightness backlight colour: NVIS green, same as NVIS_GREEN_A in 2A13 Colors.h.
#endif

static CRGB ohWs2812BacklightPixels[OH_WS2812_COUNT];  ///< Pixel buffer for the backlight string.

/**
 * @brief Register the backlight string with FastLED and switch it off. Call from setup() before OhBacklightDiagnostics::begin().
 */
static inline void ohWs2812BacklightBegin() {
  FastLED.addLeds<WS2812B, OH_WS2812_PIN, OH_WS2812_ORDER>(ohWs2812BacklightPixels, OH_WS2812_COUNT);
  FastLED.clear(true);
}

/**
 * @brief Writer for OhBacklightDiagnostics: shows OH_WS2812_COLOR scaled by @p level on every pixel.
 * @param level 0 = off, 255 = full brightness.
 */
static void ohWs2812BacklightWrite(uint8_t level) {
  CRGB colour = OH_WS2812_COLOR;
  colour.nscale8(level);
  fill_solid(ohWs2812BacklightPixels, OH_WS2812_COUNT, colour);
  FastLED.show();
}

#endif  // OPENHORNET_WS2812_BACKLIGHT_H

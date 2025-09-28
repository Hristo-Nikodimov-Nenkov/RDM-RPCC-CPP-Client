#pragma once
#include <Arduino.h>
#include <stdint.h> // for int8_t

struct LedStatus {
    int8_t code;       // signed 8-bit status code (e.g. 1, 2, -1 etc.)
    String text;       // descriptive text, e.g. "WiFi Connected"
    uint32_t color;    // NeoPixel color (24-bit RGB)

    LedStatus(int8_t code = 0, const String& text = "", uint32_t color = 0)
        : code(code), text(text), color(color) {}
};

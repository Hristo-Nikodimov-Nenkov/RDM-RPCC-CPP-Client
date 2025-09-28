#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedStatus.h"

#define NUM_STATUS_LEDS 3

enum StatusLedIndex {
    WIFI_LED = 0,
    PSU_LED = 1,
    PCSTATE_LED = 2
};

class StatusStrip {
public:
    StatusStrip(uint8_t pin, uint8_t count = NUM_STATUS_LEDS);

    void begin();
    void set(StatusLedIndex index, const LedStatus& status);
    void update(); // apply all changes
    void off();    // turn off all LEDs
    void setBrightness(uint8_t brightness); // brightness: 0–255

    // Convenience shortcuts
    void setWiFi(const LedStatus& status);
    void setSSR(const LedStatus& status);
    void setPCState(const LedStatus& status);

private:
    Adafruit_NeoPixel _strip;
    LedStatus _statuses[NUM_STATUS_LEDS];
};

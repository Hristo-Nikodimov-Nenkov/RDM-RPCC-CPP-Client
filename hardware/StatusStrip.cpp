#include "StatusStrip.h"

StatusStrip::StatusStrip(uint8_t pin, uint8_t count)
    : _strip(count, pin, NEO_GRB + NEO_KHZ800) {}

void StatusStrip::begin() {
    _strip.begin();
    _strip.clear();
    _strip.show();
}

void StatusStrip::set(StatusLedIndex index, const LedStatus& status) {
    if (index >= NUM_STATUS_LEDS) return;
    _statuses[index] = status;
    _strip.setPixelColor(index, status.color);
}

void StatusStrip::update() {
    _strip.show();
}

void StatusStrip::off() {
    _strip.clear();
    _strip.show();
}

void StatusStrip::setBrightness(uint8_t brightness) {
    _strip.setBrightness(brightness);  // Apply brightness scaling
}

void StatusStrip::setWiFi(const LedStatus& status) {
    set(WIFI_LED, status);
}

void StatusStrip::setSSR(const LedStatus& status) {
    set(SSR_LED, status);
}

void StatusStrip::setPCState(const LedStatus& status) {
    set(PCSTATE_LED, status);
}

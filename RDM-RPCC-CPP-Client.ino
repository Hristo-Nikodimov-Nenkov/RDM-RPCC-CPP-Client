#include <Arduino.h>
#include "system/ConfigurationManager.h"
#include "hardware/Display.h"
#include "hardware/StatusStrip.h"
#include "hardware/LedStatusPresets.h"

ConfigurationManager configManager;
Display display;
StatusStrip* statusStrip = nullptr;

void setup() {
    Serial.begin(115200);

    if (!configManager.load()) {
        Serial.println("Failed to load configuration");
        while (true); // Halt
    }

    auto& config = configManager.get();

    // Initialize StatusStrip
    statusStrip = new StatusStrip(config.statusStrip.pin, config.statusStrip.length);
    statusStrip->begin();

    int brightness = constrain(int(config.statusStrip.intensity * 255), 0, 255);
    statusStrip->setBrightness(brightness);

    statusStrip->setWiFi(WIFI_CONNECTING);
    statusStrip->setSSR(SSR_OFF);
    statusStrip->setPCState(PC_OFF);
    statusStrip->update();

    // Initialize Display
    if (!display.begin(config.display.width, config.display.height, config.display.scl, config.display.sda, config.display.color)) {
        Serial.println("SSD1306 initialization failed!");
        while (true);
    }

    display.clear();
    display.setCursor(0, 0);
    display.println("System Initialized");
    display.update();

    // Continue with other hardware initialization...
}

void loop() {
    // Your main loop code here
}

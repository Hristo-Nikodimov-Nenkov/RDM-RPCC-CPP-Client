#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

class ExternalMemory {
public:
    ExternalMemory(uint8_t csPin, uint8_t misoPin, uint8_t mosiPin, uint8_t sckPin);

    bool begin();
    bool isMounted() const;

    // Optional helpers
    void listFiles(const char* path = "/card", uint8_t levels = 1);
    bool remount();

private:
    uint8_t _csPin, _misoPin, _mosiPin, _sckPin;
    SPIClass _spi;

    bool _mounted;

    void printDirectory(File dir, int numTabs);
};

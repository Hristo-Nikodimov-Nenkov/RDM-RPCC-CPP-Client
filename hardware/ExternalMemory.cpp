#include "ExternalMemory.h"

ExternalMemory::ExternalMemory(uint8_t csPin, uint8_t misoPin, uint8_t mosiPin, uint8_t sckPin)
    : _csPin(csPin),
      _misoPin(misoPin),
      _mosiPin(mosiPin),
      _sckPin(sckPin),
      _spi(SPIClass(HSPI)),
      _mounted(false)
{}

bool ExternalMemory::begin() {
    _spi.begin(_sckPin, _misoPin, _mosiPin, _csPin);

    if (!SD.begin(_csPin, _spi)) {
        Serial.println("[SD] Mounting failed!");
        _mounted = false;
        return false;
    }

    if (!SD.exists("/card")) {
        SD.mkdir("/card");
    }

    _mounted = true;
    Serial.println("[SD] Mounted to /card");
    return true;
}

bool ExternalMemory::isMounted() const {
    return _mounted;
}

bool ExternalMemory::remount() {
    SD.end();
    _mounted = false;
    delay(100);
    return begin();
}

void ExternalMemory::listFiles(const char* path, uint8_t levels) {
    if (!_mounted) {
        Serial.println("[SD] Not mounted. Cannot list files.");
        return;
    }

    File root = SD.open(path);
    if (!root || !root.isDirectory()) {
        Serial.println("[SD] Invalid path or not a directory.");
        return;
    }

    printDirectory(root, levels);
    root.close();
}

void ExternalMemory::printDirectory(File dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;

        for (int i = 0; i < numTabs; i++) Serial.print('\t');
        Serial.print(entry.name());

        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        } else {
            Serial.print("\t\t");
            Serial.println(entry.size());
        }

        entry.close();
    }
}

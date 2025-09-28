#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>

struct DisplayConfig {
    uint8_t scl = 9;
    uint8_t sda = 10;
    uint8_t color = 1;
    uint16_t width = 128;
    uint16_t height = 64;
};

struct ExtStorageConfig {
    uint8_t cs = 7;
    uint8_t sck = 4;
    uint8_t miso = 5;
    uint8_t mosi = 6;
    String path = "/card";
};

struct StatusStripConfig {
    uint8_t pin = 3;
    uint8_t length = 3;
    float intensity = 0.5f;
};

struct DeviceConfig {
    String deviceId = "";
    String ownerId = "";
};

struct WifiConfig {
    String essid = "";
    String password = "";
};

struct Configuration {
    DeviceConfig device;
    WifiConfig wifi;
    DisplayConfig display;
    ExtStorageConfig extStorage;
    StatusStripConfig statusStrip;
};

class ConfigurationManager {
public:
    ConfigurationManager(const char* configPath = "/configuration.json");

    bool load();
    bool save();

    Configuration& get();
    void set(const Configuration& config);

private:
    String _configPath;
    Configuration _config;

    bool moveFromCard();
};

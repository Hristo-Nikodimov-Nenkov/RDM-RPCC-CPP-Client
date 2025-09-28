#include "ConfigurationManager.h"
#include <SPIFFS.h>

ConfigurationManager::ConfigurationManager(const char* configPath) : _configPath(configPath) {}

bool ConfigurationManager::moveFromCard() {
    if (!SPIFFS.begin()) return false;
    if (!SPIFFS.exists("/card/configuration.json")) return false;

    File src = SPIFFS.open("/card/configuration.json", FILE_READ);
    if (!src) return false;

    File dst = SPIFFS.open(_configPath, FILE_WRITE);
    if (!dst) {
        src.close();
        return false;
    }

    while (src.available()) {
        dst.write(src.read());
    }
    src.close();
    dst.close();

    SPIFFS.remove("/card/configuration.json");
    return true;
}

bool ConfigurationManager::load() {
    if (!SPIFFS.begin()) return false;

    if (!SPIFFS.exists(_configPath)) {
        // Try move from card
        if (!moveFromCard()) return false;
    }

    File file = SPIFFS.open(_configPath, FILE_READ);
    if (!file) return false;

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) return false;

    // Parse device
    if (doc.containsKey("device")) {
        auto d = doc["device"];
        _config.device.deviceId = d["deviceId"] | "";
        _config.device.ownerId = d["ownerId"] | "";
    }

    // Parse wifi
    if (doc.containsKey("wifi")) {
        auto w = doc["wifi"];
        _config.wifi.essid = w["essid"] | "";
        _config.wifi.password = w["password"] | "";
    }

    // Parse display
    if (doc.containsKey("display")) {
        auto dis = doc["display"];
        _config.display.scl = dis["scl"] | 9;
        _config.display.sda = dis["sda"] | 10;
        _config.display.color = dis["color"] | 1;
        _config.display.width = dis["width"] | 128;
        _config.display.height = dis["height"] | 64;
    }

    // Parse extStorage
    if (doc.containsKey("extStorage")) {
        auto es = doc["extStorage"];
        _config.extStorage.cs = es["cs"] | 7;
        _config.extStorage.sck = es["sck"] | 4;
        _config.extStorage.miso = es["miso"] | 5;
        _config.extStorage.mosi = es["mosi"] | 6;
        _config.extStorage.path = es["path"] | "/card";
    }

    // Parse statusStrip
    if (doc.containsKey("statusStrip")) {
        auto ss = doc["statusStrip"];
        _config.statusStrip.pin = ss["pin"] | 3;
        _config.statusStrip.length = ss["length"] | 3;
        _config.statusStrip.intensity = ss["intensity"] | 0.5f;
    }

    return true;
}

bool ConfigurationManager::save() {
    if (!SPIFFS.begin()) return false;

    DynamicJsonDocument doc(1024);

    auto d = doc.createNestedObject("device");
    d["deviceId"] = _config.device.deviceId;
    d["ownerId"] = _config.device.ownerId;

    auto w = doc.createNestedObject("wifi");
    w["essid"] = _config.wifi.essid;
    w["password"] = _config.wifi.password;

    auto dis = doc.createNestedObject("display");
    dis["scl"] = _config.display.scl;
    dis["sda"] = _config.display.sda;
    dis["color"] = _config.display.color;
    dis["width"] = _config.display.width;
    dis["height"] = _config.display.height;

    auto es = doc.createNestedObject("extStorage");
    es["cs"] = _config.extStorage.cs;
    es["sck"] = _config.extStorage.sck;
    es["miso"] = _config.extStorage.miso;
    es["mosi"] = _config.extStorage.mosi;
    es["path"] = _config.extStorage.path;

    auto ss = doc.createNestedObject("statusStrip");
    ss["pin"] = _config.statusStrip.pin;
    ss["length"] = _config.statusStrip.length;
    ss["intensity"] = _config.statusStrip.intensity;

    File file = SPIFFS.open(_configPath, FILE_WRITE);
    if (!file) return false;

    if (serializeJson(doc, file) == 0) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

Configuration& ConfigurationManager::get() {
    return _config;
}

void ConfigurationManager::set(const Configuration& config) {
    _config = config;
}

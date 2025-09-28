#include "Display.h"
#include <Wire.h>

Display::Display() : _display(nullptr), _width(0), _height(0), _color(1), _background(0) {}

Display::~Display() {
    if (_display) {
        delete _display;
        _display = nullptr;
    }
}

bool Display::begin(uint16_t width, uint16_t height, uint8_t scl, uint8_t sda, uint8_t color) {
    _width = width;
    _height = height;
    _color = color;
    _background = (color == 1) ? 0 : 1;

    Wire.begin(sda, scl);

    if (_display) {
        delete _display;
        _display = nullptr;
    }

    _display = new Adafruit_SSD1306(_width, _height, &Wire);

    if (!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return false;
    }

    clear();
    setTextColor(_color, _background);
    setCursor(0, 0);
    update();

    return true;
}

void Display::clear() {
    if (_display) {
        _display->clearDisplay();
    }
}

void Display::setTextColor(uint16_t color, uint16_t bg) {
    if (_display) {
        _color = color;
        _background = bg;
        _display->setTextColor(color, bg);
    }
}

void Display::setCursor(int16_t x, int16_t y) {
    if (_display) {
        _display->setCursor(x, y);
    }
}

void Display::print(const char* text) {
    if (_display) {
        _display->print(text);
    }
}

void Display::println(const char* text) {
    if (_display) {
        _display->println(text);
    }
}

void Display::update() {
    if (_display) {
        _display->display();
    }
}

uint16_t Display::getWidth() const {
    return _width;
}

uint16_t Display::getHeight() const {
    return _height;
}

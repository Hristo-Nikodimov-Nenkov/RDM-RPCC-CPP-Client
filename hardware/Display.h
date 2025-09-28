#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class Display {
public:
    Display();
    ~Display();

    bool begin(uint16_t width, uint16_t height, uint8_t scl, uint8_t sda, uint8_t color = 1);

    void clear();
    void setTextColor(uint16_t color, uint16_t bg = 0);
    void setCursor(int16_t x, int16_t y);
    void print(const char* text);
    void println(const char* text);
    void update();

    uint16_t getWidth() const;
    uint16_t getHeight() const;

private:
    Adafruit_SSD1306* _display;
    uint16_t _width;
    uint16_t _height;
    uint8_t _color;
    uint8_t _background;
};

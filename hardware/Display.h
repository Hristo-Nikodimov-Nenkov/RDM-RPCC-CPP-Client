#pragma once

#include <Arduino.h>
#include <TaskScheduler.h>
#include <Adafruit_SSD1306.h>

class Display {
public:
    Display(uint8_t width = 128, uint8_t height = 64, TwoWire *wire = &Wire);

    bool begin();

    // Sync methods
    void setCursor(uint8_t x, uint8_t y);
    void print(const char* text);
    void print(const String &text);

    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h);
    void clear();
    void update(); // replaces display()

    // Async methods
    void printAsync(Scheduler &scheduler, const char* text);
    void printAsync(Scheduler &scheduler, const String &text);

    void drawBitmapAsync(Scheduler &scheduler, int16_t x, int16_t y,
                         const uint8_t *bitmap, int16_t w, int16_t h);

    void clearAsync(Scheduler &scheduler);
    void updateAsync(Scheduler &scheduler); // replaces displayAsync

    // Color handling
    void setColor(uint16_t colorValue);
    void setBackground(uint16_t bgValue);

private:
    Adafruit_SSD1306 oled;

    uint8_t cursorX;
    uint8_t cursorY;

    uint16_t color = SSD1306_WHITE;
    uint16_t background = SSD1306_BLACK;

    // Async task and params
    Task asyncTask;

    enum class AsyncOperation {
        None,
        PrintText,
        DrawBitmap,
        Clear,
        Update
    } pendingOperation;

    // For print async
    String asyncText;

    // For bitmap async
    int16_t asyncX, asyncY, asyncW, asyncH;
    const uint8_t *asyncBitmap;

    static void asyncTaskCallback(Task*);
    void runAsyncOperation();
    void resetAsyncParams();
};

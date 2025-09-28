#include "Display.h"

Display::Display(uint8_t width, uint8_t height, TwoWire *wire)
    : oled(width, height, wire, -1),
      cursorX(0),
      cursorY(0),
      asyncTask(0, TASK_ONCE, asyncTaskCallback, this),
      pendingOperation(AsyncOperation::None),
      asyncBitmap(nullptr)
{}

bool Display::begin() {
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        return false;
    }

    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(color, background);
    setCursor(0, 0);
    oled.update(); // initial update
    return true;
}

// ======= Sync methods =======

void Display::setCursor(uint8_t x, uint8_t y) {
    cursorX = x;
    cursorY = y;
    oled.setCursor(x, y);
}

void Display::print(const char* text) {
    oled.setCursor(cursorX, cursorY);
    oled.setTextColor(color, background);
    oled.print(text);
    oled.update();

    cursorX = oled.getCursorX();
    cursorY = oled.getCursorY();
}

void Display::print(const String &text) {
    print(text.c_str());
}

void Display::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h) {
    oled.drawBitmap(x, y, bitmap, w, h, color);
    oled.update();
}

void Display::clear() {
    oled.clearDisplay();
}

void Display::update() {
    oled.update();
}

// ======= Async methods =======

void Display::printAsync(Scheduler &scheduler, const char* text) {
    if (!asyncTask.isEnabled()) {
        pendingOperation = AsyncOperation::PrintText;
        asyncText = String(text);
        scheduler.addTask(asyncTask);
        asyncTask.enableDelayed(0);
    }
}

void Display::printAsync(Scheduler &scheduler, const String &text) {
    printAsync(scheduler, text.c_str());
}

void Display::drawBitmapAsync(Scheduler &scheduler, int16_t x, int16_t y,
                              const uint8_t *bitmap, int16_t w, int16_t h) {
    if (!asyncTask.isEnabled()) {
        pendingOperation = AsyncOperation::DrawBitmap;
        asyncX = x;
        asyncY = y;
        asyncBitmap = bitmap;
        asyncW = w;
        asyncH = h;

        scheduler.addTask(asyncTask);
        asyncTask.enableDelayed(0);
    }
}

void Display::clearAsync(Scheduler &scheduler) {
    if (!asyncTask.isEnabled()) {
        pendingOperation = AsyncOperation::Clear;
        scheduler.addTask(asyncTask);
        asyncTask.enableDelayed(0);
    }
}

void Display::updateAsync(Scheduler &scheduler) {
    if (!asyncTask.isEnabled()) {
        pendingOperation = AsyncOperation::Update;
        scheduler.addTask(asyncTask);
        asyncTask.enableDelayed(0);
    }
}

// ======= Color API =======

void Display::setColor(uint16_t colorValue) {
    color = colorValue;
    background = (color == SSD1306_WHITE) ? SSD1306_BLACK : SSD1306_WHITE;
    oled.setTextColor(color, background);
}

void Display::setBackground(uint16_t bgValue) {
    background = bgValue;
    color = (background == SSD1306_WHITE) ? SSD1306_BLACK : SSD1306_WHITE;
    oled.setTextColor(color, background);
}

// ======= Internal =======

void Display::asyncTaskCallback(Task* task) {
    Display* self = static_cast<Display*>(task->getData());
    self->runAsyncOperation();
    task->disable();
}

void Display::runAsyncOperation() {
    oled.setTextColor(color, background);

    switch (pendingOperation) {
        case AsyncOperation::PrintText:
            oled.setCursor(cursorX, cursorY);
            oled.print(asyncText);
            oled.update();
            cursorX = oled.getCursorX();
            cursorY = oled.getCursorY();
            break;

        case AsyncOperation::DrawBitmap:
            oled.drawBitmap(asyncX, asyncY, asyncBitmap, asyncW, asyncH, color);
            oled.update();
            break;

        case AsyncOperation::Clear:
            oled.clearDisplay();
            break;

        case AsyncOperation::Update:
            oled.update();
            break;

        default:
            break;
    }

    resetAsyncParams();
}

void Display::resetAsyncParams() {
    pendingOperation = AsyncOperation::None;
    asyncText = "";
    asyncBitmap = nullptr;
}

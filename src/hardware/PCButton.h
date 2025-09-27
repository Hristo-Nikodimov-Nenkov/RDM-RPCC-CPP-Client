#pragma once

#include <Arduino.h>
#include <TaskScheduler.h>

class PCButton {
public:
    explicit PCButton(uint8_t pin);

    // Synchronous press (blocking)
    void press(float time = 0.5f);

    // Async press (schedule press task)
    void pressAsync(Scheduler &scheduler, float time = 0.5f);

private:
    uint8_t pin;

    // Task for async press
    Task pressTask;

    // For storing press duration in async
    float pressTime;

    // Static task callback
    static void pressTaskCallback(Task*);

    // Helper method to do press (pin LOW -> HIGH)
    void doPress(float time);

    // Friend to access private members in callback
    friend void pressTaskCallback(Task*);
};

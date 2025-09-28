#pragma once

#include <Arduino.h>
#include <TaskScheduler.h>
#include "PCState.h"

class PSU {
public:
    PSU(uint8_t psuPin, PCState &stateSensor);

    void turnOn();
    void turnOff();

    // Async versions
    void turnOnAsync(Scheduler &scheduler);
    void turnOffAsync(Scheduler &scheduler);

    bool isOn() const;

private:
    uint8_t pin;
    PCState &state;

    Task turnOnTask;
    Task turnOffTask;

    static void turnOnCallback(Task*);
    static void turnOffCallback(Task*);
};

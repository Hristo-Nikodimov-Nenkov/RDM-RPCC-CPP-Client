#pragma once

#include <Arduino.h>
#include <TaskScheduler.h>

class PCState {
public:
    explicit PCState(uint8_t pin);

    // Synchronous read
    bool isOn() const;

    // Async start monitoring (poll state periodically)
    void startMonitoring(Scheduler &scheduler, uint16_t intervalMs = 1000);

    // Async stop monitoring
    void stopMonitoring();

    // Get last known state as string
    String getStateString() const;

    // Register callback on state change (optional)
    typedef void (*StateChangeCallback)(bool newState);
    void setCallback(StateChangeCallback cb);

private:
    uint8_t pin;
    volatile bool currentState;

    Task pollTask;

    StateChangeCallback callback;

    static void pollTaskCallback(Task*);

    void pollState();
};

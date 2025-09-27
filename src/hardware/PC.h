#pragma once

#include <Arduino.h>
#include <TaskScheduler.h>
#include "PCButton.h"
#include "PCState.h"
#include "PSU.h"

class PC {
public:
    PC(Scheduler &scheduler,
       uint8_t powerBtnPin = 20,
       uint8_t resetBtnPin = 21,
       uint8_t statePin = 2,
       uint8_t psuPin = 1);

    // Sync
    void powerOn();
    bool powerOff();

    void shutdown();
    bool shutdownAndPowerOff(float delaySeconds = 1.0f);

    void reset();

    void hardShutdown();
    bool hardShutdownAndPowerOff(float delaySeconds = 1.0f);

    // Async (schedule tasks)
    void powerOnAsync();
    void powerOnAndStartAsync(float delaySeconds = 1.0f);

    void powerOffAsync();

    void shutdownAsync();
    void shutdownAndPowerOffAsync(float delaySeconds = 1.0f);

    void resetAsync();

    void hardShutdownAsync();
    void hardShutdownAndPowerOffAsync(float delaySeconds = 1.0f);

    // State
    bool isRunning() const;
    String getStateString() const;

    // Force
    void forceShutdown();

private:
    // Hardware
    PCButton powerButton;
    PCButton resetButton;
    PCState stateSensor;
    PSU psuSwitch;

    // Scheduler ref
    Scheduler &scheduler;

    // Task objects
    Task taskPowerOn;
    Task taskPowerOnAndStart;
    Task taskPowerOff;
    Task taskShutdown;
    Task taskShutdownAndPowerOff;
    Task taskReset;
    Task taskHardShutdown;
    Task taskHardShutdownAndPowerOff;

    // Async helpers and state
    float asyncDelaySeconds = 1.0f;

    // Task callbacks
    static void powerOnCallback(Task*);
    static void powerOnAndStartCallback(Task*);
    static void powerOffCallback(Task*);
    static void shutdownCallback(Task*);
    static void shutdownAndPowerOffCallback(Task*);
    static void resetCallback(Task*);
    static void hardShutdownCallback(Task*);
    static void hardShutdownAndPowerOffCallback(Task*);
};

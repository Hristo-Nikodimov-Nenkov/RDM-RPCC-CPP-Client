#include "PC.h"

PC::PC(Scheduler &sched,
       uint8_t powerBtnPin,
       uint8_t resetBtnPin,
       uint8_t statePin,
       uint8_t psuPin)
    : scheduler(sched),
      powerButton(powerBtnPin),
      resetButton(resetBtnPin),
      stateSensor(statePin),
      psuSwitch(psuPin, stateSensor),

      // Initialize Tasks with zero interval and callback (disabled by default)
      taskPowerOn(0, TASK_ONCE, powerOnCallback, this),
      taskPowerOnAndStart(0, TASK_ONCE, powerOnAndStartCallback, this),
      taskPowerOff(0, TASK_ONCE, powerOffCallback, this),
      taskShutdown(0, TASK_ONCE, shutdownCallback, this),
      taskShutdownAndPowerOff(0, TASK_ONCE, shutdownAndPowerOffCallback, this),
      taskReset(0, TASK_ONCE, resetCallback, this),
      taskHardShutdown(0, TASK_ONCE, hardShutdownCallback, this),
      taskHardShutdownAndPowerOff(0, TASK_ONCE, hardShutdownAndPowerOffCallback, this)
{
    scheduler.addTask(taskPowerOn);
    scheduler.addTask(taskPowerOnAndStart);
    scheduler.addTask(taskPowerOff);
    scheduler.addTask(taskShutdown);
    scheduler.addTask(taskShutdownAndPowerOff);
    scheduler.addTask(taskReset);
    scheduler.addTask(taskHardShutdown);
    scheduler.addTask(taskHardShutdownAndPowerOff);
}

// ===== Sync implementations =====

void PC::powerOn() {
    psuSwitch.turnOn();
    delay(500);
}

bool PC::powerOff() {
    return psuSwitch.turnOff();
}

void PC::shutdown() {
    powerButton.press();
}

bool PC::shutdownAndPowerOff(float delaySeconds) {
    shutdown();

    unsigned long start = millis();
    const unsigned long timeout = 30000;
    while (isRunning()) {
        if (millis() - start > timeout) {
            Serial.println("[PC] Shutdown timeout.");
            return false;
        }
        delay(200);
    }

    delay((int)(delaySeconds * 1000));
    return powerOff();
}

void PC::reset() {
    resetButton.press();
}

void PC::hardShutdown() {
    powerButton.press(5.0f);  // Hold power button 5 seconds

    unsigned long start = millis();
    const unsigned long timeout = 15000;
    while (isRunning()) {
        if (millis() - start > timeout) {
            Serial.println("[PC] Hard shutdown timeout.");
            break;
        }
        delay(200);
    }
}

bool PC::hardShutdownAndPowerOff(float delaySeconds) {
    hardShutdown();

    delay((int)(delaySeconds * 1000));
    return powerOff();
}

// ===== Async methods schedule tasks =====

void PC::powerOnAsync() {
    if (!taskPowerOn.isEnabled()) {
        taskPowerOn.enableDelayed(0);
    }
}

void PC::powerOnAndStartAsync(float delaySeconds) {
    asyncDelaySeconds = delaySeconds;
    if (!taskPowerOnAndStart.isEnabled()) {
        taskPowerOnAndStart.enableDelayed(0);
    }
}

void PC::powerOffAsync() {
    if (!taskPowerOff.isEnabled()) {
        taskPowerOff.enableDelayed(0);
    }
}

void PC::shutdownAsync() {
    if (!taskShutdown.isEnabled()) {
        taskShutdown.enableDelayed(0);
    }
}

void PC::shutdownAndPowerOffAsync(float delaySeconds) {
    asyncDelaySeconds = delaySeconds;
    if (!taskShutdownAndPowerOff.isEnabled()) {
        taskShutdownAndPowerOff.enableDelayed(0);
    }
}

void PC::resetAsync() {
    if (!taskReset.isEnabled()) {
        taskReset.enableDelayed(0);
    }
}

void PC::hardShutdownAsync() {
    if (!taskHardShutdown.isEnabled()) {
        taskHardShutdown.enableDelayed(0);
    }
}

void PC::hardShutdownAndPowerOffAsync(float delaySeconds) {
    asyncDelaySeconds = delaySeconds;
    if (!taskHardShutdownAndPowerOff.isEnabled()) {
        taskHardShutdownAndPowerOff.enableDelayed(0);
    }
}

// ===== Task Callbacks =====

void PC::powerOnCallback(Task* t) {
    PC* self = static_cast<PC*>(t->getData());
    self->powerOn();
    t->disable();
}

void PC::powerOnAndStartCallback(Task

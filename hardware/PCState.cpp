#include "PCState.h"

PCState::PCState(uint8_t pinNumber)
    : pin(pinNumber),
      currentState(false),
      pollTask(1000, TASK_FOREVER, pollTaskCallback, this),
      callback(nullptr)
{
    pinMode(pin, INPUT_PULLUP);
    currentState = digitalRead(pin) == LOW; // Active LOW assumed for ON
}

bool PCState::isOn() const {
    return currentState;
}

void PCState::startMonitoring(Scheduler &scheduler, uint16_t intervalMs) {
    if (!pollTask.isEnabled()) {
        pollTask.setInterval(intervalMs);
        scheduler.addTask(pollTask);
        pollTask.enable();
    }
}

void PCState::stopMonitoring() {
    pollTask.disable();
}

void PCState::setCallback(StateChangeCallback cb) {
    callback = cb;
}

String PCState::getStateString() const {
    return currentState ? "ON" : "OFF";
}

void PCState::pollTaskCallback(Task* task) {
    PCState* self = static_cast<PCState*>(task->getData());
    self->pollState();
}

void PCState::pollState() {
    bool stateNow = digitalRead(pin) == LOW; // Active LOW = ON

    if (stateNow != currentState) {
        currentState = stateNow;
        if (callback) callback(currentState);
    }
}

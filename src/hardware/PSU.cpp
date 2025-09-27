#include "PSU.h"

PSU::PSU(uint8_t psuPin, PCState &stateSensor)
    : pin(psuPin),
      state(stateSensor),
      turnOnTask(0, TASK_ONCE, turnOnCallback, this),
      turnOffTask(0, TASK_ONCE, turnOffCallback, this)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);  // Initially off
}

void PSU::turnOn() {
    digitalWrite(pin, HIGH);
}

void PSU::turnOff() {
    // Safety: Only turn off if PC state is OFF
    if (!state.isOn()) {
        digitalWrite(pin, LOW);
    }
}

void PSU::turnOnAsync(Scheduler &scheduler) {
    if (!turnOnTask.isEnabled()) {
        scheduler.addTask(turnOnTask);
        turnOnTask.enableDelayed(0);
    }
}

void PSU::turnOffAsync(Scheduler &scheduler) {
    if (!turnOffTask.isEnabled()) {
        scheduler.addTask(turnOffTask);
        turnOffTask.enableDelayed(0);
    }
}

bool PSU::isOn() const {
    return digitalRead(pin) == HIGH;
}

void PSU::turnOnCallback(Task* task) {
    PSU* self = static_cast<PSU*>(task->getData());
    self->turnOn();
    task->disable();
}

void PSU::turnOffCallback(Task* task) {
    PSU* self = static_cast<PSU*>(task->getData());
    self->turnOff();
    task->disable();
}

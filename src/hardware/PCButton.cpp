#include "PCButton.h"

PCButton::PCButton(uint8_t pinNumber)
    : pin(pinNumber),
      pressTask(0, TASK_ONCE, pressTaskCallback, this), // task interval 0, run once
      pressTime(0.5f)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);  // Initially HIGH (not pressed)
}

void PCButton::press(float time) {
    doPress(time);
}

void PCButton::pressAsync(Scheduler &scheduler, float time) {
    if (!pressTask.isEnabled()) {
        pressTime = time;
        scheduler.addTask(pressTask);
        pressTask.enableDelayed(0);
    }
}

void PCButton::doPress(float time) {
    digitalWrite(pin, LOW);
    delay((int)(time * 1000));
    digitalWrite(pin, HIGH);
}

void PCButton::pressTaskCallback(Task* task) {
    PCButton* self = static_cast<PCButton*>(task->getData());
    self->doPress(self->pressTime);
    task->disable();
}

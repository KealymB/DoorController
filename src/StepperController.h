#ifndef STEPPERCONTROLLER_H
#define STEPPERCONTROLLER_H

#include <Stepper.h>

class StepperController
{
public:
    StepperController(int pin1, int pin2, int pin3, int pin4, int speed = 15, int stepsPerRevolution = 2048);
    void openLock();
    void closeLock();
    void setRange(int minimumRange, int maximumRange);

private:
    Stepper stepperMotor;
    int motorPins[4];
    int stepsPerRevolution;
    int currentStepCount;
    int minimumRange;
    int maximumRange;
    void moveTo(int position);
    void sleep();
};

#endif
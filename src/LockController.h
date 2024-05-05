#ifndef LOCKCONTROLLER_H
#define LOCKCONTROLLER_H

#include <Stepper.h>

class LockController
{
public:
    LockController();
    void setup(int pin1, int pin2, int pin3, int pin4, int speed = 10, int stepsPerRevolution = 2048);
    void openLock();
    void closeLock();
    void update();
    void setRange(int minimumRange, int maximumRange);
    virtual ~LockController();

private:
    Stepper *stepperMotor;
    int motorPins[4];
    int stepsPerRevolution;
    int currentStepCount;
    int minimumRange;
    int maximumRange;
    bool isMoving;
    bool isLocked;
    unsigned long moveStartTime;
    unsigned long movementDuration = 10000;
    void moveTo(int position);
    void checkMovement();
    void sleep();
};

#endif

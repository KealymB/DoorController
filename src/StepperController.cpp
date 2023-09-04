#include <Arduino.h>
#include "StepperController.h"
#include <Stepper.h>

StepperController::StepperController(int pin1, int pin2, int pin3, int pin4, int speed, int stepsPerRevolution)
    : stepperMotor(stepsPerRevolution, pin1, pin2, pin3, pin4), stepsPerRevolution(stepsPerRevolution)
{
    stepperMotor.setSpeed(speed);

    motorPins[0] = pin1;
    motorPins[1] = pin2;
    motorPins[2] = pin3;
    motorPins[3] = pin4;
}
void StepperController::openLock()
{
    moveTo(maximumRange);
}

void StepperController::closeLock()
{
    moveTo(minimumRange);
}

void StepperController::moveTo(int position)
{
    int numberOfStepsToMove = currentStepCount - position;
    bool shouldMove = numberOfStepsToMove != 0;

    if (shouldMove)
    {
        stepperMotor.step(numberOfStepsToMove);
        currentStepCount = position;
    }
}

// Implement the setRange function
void StepperController::setRange(int minimumRange, int maximumRange)
{
    // Implement the logic to set the range here
    this->minimumRange = minimumRange;
    this->maximumRange = maximumRange;
}

void StepperController::sleep()
{
    for (int currentPinIndex = 0; currentPinIndex < sizeof(motorPins) / sizeof(motorPins[0]); currentPinIndex++)
    {
        digitalWrite(currentPinIndex, LOW);
    }
}
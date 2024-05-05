#include <Arduino.h>
#include <LockController.h>

LockController::LockController()
{
    stepperMotor = nullptr;
}

void LockController::setup(int pin1, int pin2, int pin3, int pin4, int speed, int stepsPerRevolution)
{
    Serial.println("Initialising stepper motor");

    stepperMotor = new Stepper(stepsPerRevolution, pin1, pin2, pin3, pin4);

    stepperMotor->setSpeed(speed);

    motorPins[0] = pin1;
    motorPins[1] = pin2;
    motorPins[2] = pin3;
    motorPins[3] = pin4;
}
void LockController::openLock()
{
    Serial.println("opening");
    moveTo(maximumRange);
}

void LockController::closeLock()
{
    Serial.println("closing");
    moveTo(minimumRange);
}

void LockController::moveTo(int position)
{
    int numberOfStepsToMove = currentStepCount - position;
    bool shouldMove = numberOfStepsToMove != 0;

    if (shouldMove)
    {
        stepperMotor->step(numberOfStepsToMove);
        currentStepCount = position;

        isMoving = true;
        moveStartTime = millis();
    }
}

void LockController::setRange(int minimumRange, int maximumRange)
{
    this->minimumRange = minimumRange;
    this->maximumRange = maximumRange;
}

void LockController::sleep()
{
    Serial.println("Sleeping");

    for (int currentPinIndex = 0; currentPinIndex < sizeof(motorPins) / sizeof(motorPins[0]); currentPinIndex++)
    {
        digitalWrite(motorPins[currentPinIndex], LOW);
    }
}

void LockController::update()
{
    if (isMoving)
    {
        if (millis() - moveStartTime >= movementDuration)
        {
            sleep();
            isMoving = false;
        }
    }
}

LockController::~LockController()
{
    delete stepperMotor;
}
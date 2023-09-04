#include <Arduino.h>
#include <StepperController.h>
#include <BleServer.h>

StepperController stepperController(16, 17, 18, 19);
BleServer bleServer;

void onServerWrite(const char *response)
{
  Serial.print("I Got this in main: ");
  Serial.println(response);

  Serial.print("I am deciphering as: ");
  int intValue = std::stoi(response, nullptr, 16);

  // Cast the integer to a char
  char charValue = static_cast<char>(intValue);

  Serial.println(charValue);
}

void onServerRead(const char *response)
{
  Serial.print("I Got this in main:");
  Serial.println(response);
}

void setup()
{
  Serial.begin(115200);
  stepperController.setRange(-200, 1020);
  bleServer.setup(onServerWrite, onServerRead);

  Serial.println("Starting NimBLE Server");
}

void loop()
{
}
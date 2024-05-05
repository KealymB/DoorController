#include <Arduino.h>
#include <LockController.h>
#include <BleServer.h>

LockController lockController;
BleServer bleServer;

void onServerWrite(const char *response)
{
  Serial.print("Received: ");
  Serial.println(response);

  if (*response == char('\x01'))
  {
    lockController.openLock();
  }
  else if (*response == char('\x02'))
  {
    lockController.closeLock();
  }
}

void onServerRead(const char *response)
{
  // Serial.print("I Got this in main:");
}

void setup()
{
  Serial.begin(115200);

  lockController.setup(16, 17, 18, 19);
  lockController.setRange(-200, 1020);
  bleServer.setup(&onServerWrite, &onServerRead);

  Serial.println("Starting NimBLE Server");
}

void loop()
{
}
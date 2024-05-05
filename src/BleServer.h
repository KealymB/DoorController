#include <NimBLEDevice.h>

#ifndef BLESERVER_H
#define BLESERVER_H

class BleServer
{
public:
    BleServer();
    void setup(void (*onReadCallback)(const char *response),
               void (*onWriteCallback)(const char *response));

private:
    static NimBLEServer *pServer;
    class ServerCallbacks;
    class CharacteristicCallbacks;
};

#endif
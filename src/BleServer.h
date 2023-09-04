#include <NimBLEDevice.h>

#ifndef BLESERVER_H
#define BLESERVER_H

class BleServer
{
public:
    BleServer();
    void setup(std::function<void(const char *response)> onReadCallback,
               std::function<void(const char *response)> onWriteCallback);

private:
    static NimBLEServer *pServer;
    class ServerCallbacks;
    class CharacteristicCallbacks;
};

#endif
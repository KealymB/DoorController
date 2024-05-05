#include "BleServer.h"

class BleServer::ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer)
    {
        Serial.println("Client connected");
        NimBLEDevice::startAdvertising();
    };

    void onConnect(NimBLEServer *pServer, ble_gap_conn_desc *desc)
    {
        Serial.print("Client address: ");
        Serial.println(NimBLEAddress(desc->peer_ota_addr).toString().c_str());
        /** We can use the connection handle here to ask for different connection parameters.
         *  Args: connection handle, min connection interval, max connection interval
         *  latency, supervision timeout.
         *  Units; Min/Max Intervals: 1.25 millisecond increments.
         *  Latency: number of intervals allowed to skip.
         *  Timeout: 10 millisecond increments, try for 5x interval time for best results.
         */
        pServer->updateConnParams(desc->conn_handle, 24, 48, 0, 6000);
    };
    void onDisconnect(NimBLEServer *pServer)
    {
        Serial.println("Client disconnected - start advertising");
        NimBLEDevice::startAdvertising();
    };
    void onMTUChange(uint16_t MTU, ble_gap_conn_desc *desc)
    {
        Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, desc->conn_handle);
    };
};

class BleServer::CharacteristicCallbacks
    : public NimBLECharacteristicCallbacks
{
public:
    CharacteristicCallbacks(void (*onWriteCallback)(const char *response),
                            void (*onReadCallback)(const char *response))
        : onWriteCallback_(onWriteCallback), onReadCallback_(onReadCallback) {}

    void onRead(NimBLECharacteristic *pCharacteristic)
    {
        const std::string &characteristicValue = pCharacteristic->getValue();

        if (!characteristicValue.empty())
        {
            const char *response = characteristicValue.c_str();
            (*onReadCallback_)(response);
        }
    };

    void onWrite(NimBLECharacteristic *pCharacteristic)
    {
        const std::string &characteristicValue = pCharacteristic->getValue();

        if (!characteristicValue.empty())
        {
            const char *response = characteristicValue.c_str();
            (*onWriteCallback_)(response);
        }
    };

private:
    void (*onReadCallback_)(const char *response);
    void (*onWriteCallback_)(const char *response);
};

NimBLEServer *BleServer::pServer = nullptr;

BleServer::BleServer()
{
}

void BleServer::setup(void (*onReadCallback)(const char *response),
                      void (*onWriteCallback)(const char *response))
{
    Serial.println("Starting BLE Server");

    static CharacteristicCallbacks chrCallbacks = CharacteristicCallbacks(onReadCallback, onWriteCallback);

    NimBLEDevice::init("JK House");

    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    NimBLEDevice::setSecurityAuth(true, true, true);
    NimBLEDevice::setSecurityPasskey(42069);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);

    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    NimBLEService *doorService = pServer->createService("Door");
    NimBLECharacteristic *pDoorUnlockCharacteristic = doorService->createCharacteristic("1235", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);

    doorService->start();
    pDoorUnlockCharacteristic->setValue("Locked");
    pDoorUnlockCharacteristic->setCallbacks(&chrCallbacks);

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("ABCD");
    pAdvertising->start();
}

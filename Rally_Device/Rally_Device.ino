
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//---------------------------------------------------------
// Constants
//---------------------------------------------------------
#define SERVICE_UUID        "55725ac1-066c-48b5-8700-2d9fb3603c5e"
#define CHARACTERISTIC_UUID "69ddb59c-d601-4ea4-ba83-44f679a670ba"
#define BLE_DEVICE_NAME     "Rally_Device"

const int switch_1 = 15;
const int switch_2 = 2;
const int switch_3 = 0;            
const int switch_4 = 4;
const int switch_5 = 16;
const int toggle_1 = 17;
const int toggle_2 = 5;

//---------------------------------------------------------
// Variables
//---------------------------------------------------------
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string rxValue;
std::string txValue;
bool bleOn = false;

int SW1 = 0;
int SW2 = 0;
int SW3 = 0;
int SW4 = 0;
int SW5 = 0;
int TG1 = 0;
int TG2 = 0;

//---------------------------------------------------------
// Callbacks
//---------------------------------------------------------
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("onConnect");
  };
  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("onDisconnect");
  }
};


class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    Serial.println("onWrite");
    std::string rxValue = pCharacteristic->getValue();
    if( rxValue.length() > 0 ){
      bleOn = rxValue[0]!=0;
      Serial.print("Received Value: ");
      for(int i=0; i<rxValue.length(); i++ ){
        Serial.print(rxValue[i],HEX);
      }
      Serial.println();
    }
  }
};

//---------------------------------------------------------
void setup() {

  pinMode(switch_1, INPUT_PULLUP);
  pinMode(switch_2, INPUT_PULLUP);
  pinMode(switch_3, INPUT_PULLUP);
  pinMode(switch_4, INPUT_PULLUP);
  pinMode(switch_5, INPUT_PULLUP);
  pinMode(toggle_1, INPUT_PULLUP);
  pinMode(toggle_2, INPUT_PULLUP);

  Serial.begin(115200);
  BLEDevice::init(BLE_DEVICE_NAME);

  // Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE  |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());
  //
  pService->start();
  // Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  Serial.println("startAdvertising");

}

void loop() {

  // disconnecting
  if(!deviceConnected && oldDeviceConnected){
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();
    Serial.println("restartAdvertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if(deviceConnected && !oldDeviceConnected){
    oldDeviceConnected = deviceConnected;
  }
  //Switch
  if(digitalRead(switch_1) == LOW){
    String str = "0";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

  if(digitalRead(switch_2) == LOW){
    String str = "1";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

  if(digitalRead(switch_3) == LOW){
    String str = "3";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

  if(digitalRead(switch_4) == LOW){
    String str = "2";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

  if(digitalRead(switch_5) == LOW){
    String str = "4";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

  if(digitalRead(toggle_1) == LOW){
    String str = "5";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

  if(digitalRead(toggle_2) == LOW){
    String str = "6";
    txValue = str.c_str();
    pCharacteristic->setValue(txValue);
    pCharacteristic->notify();
  }

}

#include <Arduino.h>
#include <Ticker.h>
/*
【ライセンスについて】
Copyright(c) 2022 by tamanegi
Released under the MIT license
'http://tamanegi.digick.jp/about-licence/
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BUFFER 64
#define VIB_MOTOR D0
#define STATE_BLE D1
#define VIB_SWTCH D2

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue[BUFFER] = "";
int vibflag = 0,attachFlag = 0;

Ticker nomalVib;
Ticker stepVib;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

void pattern1(void){
  digitalWrite(D0,HIGH);
  delay(300);
  digitalWrite(D0,LOW);
  attachFlag = 1;
  vibflag = 1;
}

void pattern2(void){
  digitalWrite(D0,HIGH);
  delay(1000);
  digitalWrite(D0,LOW);
  delay(1000);
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        if(rxValue[0] == 'a'){
          nomalVib.detach();
          stepVib.once(0.5,pattern2);
        }
        else if(vibflag == 1){
          stepVib.detach();
          nomalVib.attach(1,pattern1);
          vibflag = 0;
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(D0,OUTPUT);


  // Create the BLE Device
  BLEDevice::init("onestepR");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  Serial.read();
  //以下修正箇所　COMからデータを読み取り、BLEに送信
  if (deviceConnected == true && attachFlag == 0)
  {

		delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

  if (deviceConnected == false && attachFlag == 1)
  {
    nomalVib.detach();
    attachFlag == 0;
		delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		    nomalVib.attach(1,pattern1);
        oldDeviceConnected = deviceConnected;
    }
}
//03.08.22 makischu as-is
//part of direct-pv2heat-meter. bridge between BLE and Wifi.
// based on https://github.com/espressif/arduino-esp32/blob/master/libraries/AsyncUDP/examples/AsyncUDPServer/AsyncUDPServer.ino
// initially based on https://github.com/nkolban/ESP32_BLE_Arduino/tree/master/examples/BLE_client 
// later rewritten and moved to nimBLE https://github.com/h2zero/NimBLE-Arduino/blob/release/1.4/docs/New_user_guide.md#creating-a-client
// or https://github.com/h2zero/NimBLE-Arduino/blob/release/1.4/examples/NimBLE_Client/NimBLE_Client.ino
// deployed on a Firebeetle https://www.dfrobot.com/product-1590.html

#include "WiFi.h"
#include "AsyncUDP.h"
#include "NimBLEDevice.h" //#include "BLEDevice.h"
//For Watchdog example see https://iotassistant.io/esp32/enable-hardware-watchdog-timer-esp32-arduino-ide/
#include <esp_task_wdt.h>

#define WDT_TIMEOUT 10

const char * ssid = "****";
const char * password = "****";
IPAddress ip (192, 168, *, 38);
//IPAddress dns(192, 168, *, 1);   
IPAddress gw (192, 168, *, 1);   
IPAddress sub(255, 255, 255, 0);
IPAddress remoteIP;
uint16_t  remotePort;
AsyncUDP udp;
unsigned long previousMillis = 0;
unsigned long interval = 10000;
unsigned long nrWifiConnects = 0;
unsigned long nrBLEConnects = 0;
unsigned long tLastBLENotify=0;




#define LED_PWR       (2)
#define LED_BT        (5)
#define LED_WIFI      (13)


#define SERVICE_UUID                "58fad6cc-53b4-4dfd-ad3f-2072af1bbf2c"
#define CHARACTERISTIC_STATUS_UUID  "2a201432-a3c9-4b7c-b3ed-24b72d7f9158"
#define CHARACTERISTIC_CMD_UUID     "cf1ac9a7-87fe-400a-9bcf-a296dbc868c4"
// The remote service we wish to connect to. & The characteristic of the remote service we are interested in.
static NimBLEUUID    serviceUUID(SERVICE_UUID);
static NimBLEUUID    charUUIDstatus(CHARACTERISTIC_STATUS_UUID);
static NimBLEUUID    charUUIDcmd(CHARACTERISTIC_CMD_UUID);

//static BLEAdvertisedDevice*     pRemoteDevice=0;
static NimBLERemoteCharacteristic* pCharacteristicStatus=0;
static NimBLERemoteCharacteristic* pCharacteristicCmd=0;
static NimBLEClient*               pClient =0;
int BLEwriteFails=0;
long rssi_ble = -127;


uint8_t bleCmd[4];
bool    bleCmdPending=0;

uint8_t bleStatus[32];
size_t  bleStatusLen=0;
bool    bleStatusNew=0;
unsigned long tLastBLEStatusNotify=0;



void sendUDPStatus() {
    uint8_t data[64]; uint8_t* pD = data;
    long rssi_wifi = WiFi.RSSI();
    //long rssi_ble  = pRemoteDevice ? pRemoteDevice->getRSSI() : -127;   returns always the same, maybe from the time of scan.
    //long rssi_ble = pClient ? pClient->getRssi() : -127; // with this line sth stopped working... so tried same in loop.
    rssi_ble = pClient ? pClient->getRssi() : -127; //try again... now called from main loop
    uint32_t mills = millis();
    memcpy(pD,"RCVD",4); pD+=4;
    memcpy(pD,bleStatus,bleStatusLen); pD+=bleStatusLen;
    *(pD++) = (uint8_t)(rssi_wifi&0xFF);
    *(pD++) = (uint8_t)(rssi_ble&0xFF);
    *(pD++) = (uint8_t)(nrWifiConnects&0xFF);
    *(pD++) = (uint8_t)(nrBLEConnects&0xFF);
    memcpy(pD,&mills,4); pD+=4;
    size_t res = udp.writeTo(data, pD-data, remoteIP, remotePort);
    //Serial.print("UDP writeTo returned ");
    //Serial.println(res);
}

static void notifyCallback(
  NimBLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
//    Serial.print("data: ");
//    Serial.println((char*)pData);

    //preapre data for UDP frame.
    if (remotePort != 0 && length < 32 && !bleStatusNew) {
        tLastBLEStatusNotify = millis();
        memcpy(bleStatus,pData,length);
        bleStatusLen = length;
        bleStatusNew = true;
    }
}



void disconnectBLE() {
  if (pClient) {
    NimBLEDevice::deleteClient(pClient);
    pClient = 0;
    pCharacteristicStatus = 0;
    pCharacteristicCmd = 0;
  }
}

// blocking call. kiss.
void connectBLE() {
  Serial.println("connectBLE start");
  disconnectBLE();

  NimBLEScan *pScan = NimBLEDevice::getScan();
  NimBLEScanResults results = pScan->start(4);
  bool subscribed = false;
  
  for(int i = 0; i < results.getCount(); i++) {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    //Serial.print(device.getAddress().toString().c_str());
    //Serial.println(device.getName().c_str());
    
    if (strcmp("ESP32pv",device.getName().c_str()) == 0) {
    //if (device.isAdvertisingService(serviceUuid)) {
      Serial.println("found ESP32pv");
      pClient = NimBLEDevice::createClient();

      if (pClient->connect(&device)) {
        //Serial.println("connected");
        NimBLERemoteService *pService = pClient->getService(serviceUUID);
        
        if (pService != nullptr) {
            //Serial.println("found service");
            pCharacteristicStatus = pService->getCharacteristic(charUUIDstatus);
            pCharacteristicCmd    = pService->getCharacteristic(charUUIDcmd);
            
            if (pCharacteristicStatus != nullptr) {
              //Serial.println("got charac status");
              // std::string value = pCharacteristic->readValue();
              
              if(pCharacteristicStatus->canNotify()) {
                if(pCharacteristicStatus->subscribe(true, notifyCallback)) {
                  subscribed = true;
                  break;
                }
              }
            }
            if (pCharacteristicCmd != nullptr) {
              //Serial.println("got charac cmd");
            }
        }
      } else {
        Serial.println("but failed to connect");
      // failed to connect
      }
    }
  }
  if (pCharacteristicStatus == 0 || pCharacteristicCmd == 0 || subscribed == false) {
    //Serial.println("but charac is missing.... deleting client.");
    disconnectBLE();
    Serial.println("connectBLE failed");
  }
  if (pClient) {
    Serial.println("connectBLE success");
    nrBLEConnects++;
    BLEwriteFails=0;
  }
}

bool BLEwriteCMD(uint8_t* data) {
  bool wrote = false;
  size_t length = 4;
  Serial.println("BLEwriteCMD");
  if (pCharacteristicCmd) {
    wrote = pCharacteristicCmd->writeValue(data,length, true);
    if (wrote  && BLEwriteFails>0)  BLEwriteFails--;
    if (!wrote && BLEwriteFails<=9) BLEwriteFails++;
  }
  Serial.println(wrote ? "CMD written" : "CMD write failed");
  return wrote;
}

bool BLEwriteFailedTooMuch() {
  return BLEwriteFails >= 5;
}

void onRcvd(AsyncUDPPacket& packet) {
  bool res;
    Serial.print("UDP Packet Type: ");
    Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
    Serial.print(", From: ");
    Serial.print(packet.remoteIP());
    Serial.print(":");
    Serial.print(packet.remotePort());
    Serial.print(", To: ");
    Serial.print(packet.localIP());
    Serial.print(":");
    Serial.print(packet.localPort());
    Serial.print(", Length: ");
    Serial.print(packet.length());
    Serial.print(", Data: ");
    Serial.write(packet.data(), packet.length());
    Serial.println();
    //reply to the client
    //packet.printf("ACK from bridge: %u bytes", packet.length());

    if (packet.length() >= 8 && memcmp(packet.data(), "SEND", 4) ==0) {
      remoteIP    = packet.remoteIP();
      remotePort  = packet.remotePort();
      memcpy(bleCmd, &packet.data()[4], 4);
      bleCmdPending = true;
//      res = BLEwriteCMD(&packet.data()[4], 4);
//      Serial.print("CMD written");
//      Serial.println(res?1:0);
    }
}

void WiFiConnect() {
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      nrWifiConnects++;
      if(udp.listen(1234)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket(&onRcvd);
      }
    }
}


void setup()
{
    Serial.begin(115200);
    pinMode(LED_PWR, OUTPUT);
    pinMode(LED_BT, OUTPUT);
    pinMode(LED_WIFI, OUTPUT);
    digitalWrite(LED_PWR, LOW);
    digitalWrite(LED_BT,  LOW);
    digitalWrite(LED_WIFI, LOW);
    delay(1000);
    digitalWrite(LED_PWR, HIGH);
    delay(1000);
    digitalWrite(LED_BT,  HIGH);
    delay(1000);
    digitalWrite(LED_WIFI, HIGH);
    delay(1000);
    digitalWrite(LED_BT,  LOW);
    digitalWrite(LED_WIFI, LOW);

    Serial.println("Configuring WDT...");
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch

    
    WiFi.persistent(false);   // daten nicht in Flash speichern - vermeintlicher default.
    WiFi.mode(WIFI_STA);
    WiFi.config(ip,gw,sub);
    WiFiConnect();

    NimBLEDevice::init("ESP32pvbr");
}

void loop()
{
  //alle paar sekunden probiere ggf. neu mit Wifi und BLE zu verbinden.
  unsigned long currentMillis = millis();
  bool BLEok = pClient != 0 && pClient->isConnected() && !BLEwriteFailedTooMuch() && currentMillis - tLastBLEStatusNotify <=interval;
  bool WiFiok = WiFi.status() == WL_CONNECTED;
  
  digitalWrite(LED_BT,  BLEok ? HIGH : LOW);
  digitalWrite(LED_WIFI, WiFiok ? HIGH : LOW);

  if (currentMillis - previousMillis >=interval) {
    // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
    if (!WiFiok) {
      Serial.print(currentMillis);
      Serial.println("Reconnecting to WiFi...");
      WiFiConnect();
    }

    esp_task_wdt_reset();

    if (!BLEok) {
      Serial.println("Reconnecting to BLE...");
      connectBLE();
    }
  
    previousMillis = currentMillis;
  }

  // sending out received messages on the other channel
  // done in main loop (and main thread) for easier debugging
  if (bleCmdPending) {
    BLEwriteCMD(bleCmd);
    bleCmdPending = false;
    digitalWrite(LED_WIFI, LOW);
  }
  if (bleStatusNew) {
    sendUDPStatus();
    bleStatusNew = false;
    digitalWrite(LED_BT, LOW);
  }

  esp_task_wdt_reset();

  delay(10);
    
}



//03.08.22 makischu as-is
//part of direct-pv2heat-meter. bridge between BLE and Wifi.
// based on https://github.com/espressif/arduino-esp32/blob/master/libraries/AsyncUDP/examples/AsyncUDPServer/AsyncUDPServer.ino
// based on https://github.com/nkolban/ESP32_BLE_Arduino/tree/master/examples/BLE_client
// deployed on a Firebeetle https://www.dfrobot.com/product-1590.html

#include "WiFi.h"
#include "AsyncUDP.h"
#include "BLEDevice.h"
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
static BLEUUID    serviceUUID(SERVICE_UUID);
static BLEUUID    charUUIDstatus(CHARACTERISTIC_STATUS_UUID);
static BLEUUID    charUUIDcmd(CHARACTERISTIC_CMD_UUID);

static BLEAdvertisedDevice*     pRemoteDevice=0;
static BLERemoteCharacteristic* pRemoteCharacteristicStatus=0;
static BLERemoteCharacteristic* pRemoteCharacteristicCmd=0;
static BLEClient*               pClient =0;
long rssi_ble = -127;


uint8_t bleCmd[4];
bool    bleCmdPending=0;

uint8_t bleStatus[32];
size_t  bleStatusLen=0;
bool    bleStatusNew=0;
unsigned long tLastBLEStatusNotify=0;

bool scanActive=false;

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
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
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


class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    Serial.println("onDisconnect");
    //https://github.com/nkolban/esp32-snippets/issues/498
    if (pClient!=0 && pClient==pclient) {
      //pClient->disconnect();
      //delete pClient;
      pClient = 0;
    }
    pRemoteCharacteristicStatus = 0; //what about cleanup?? see above.
    pRemoteCharacteristicCmd = 0; //what about cleanup?? see above.
    delete pRemoteDevice; // on disconnect, also force a scan (instead of just trying to reconnect)
    pRemoteDevice = 0; //what about cleanup??
  }
};

bool connectToServer() {
    if (!pRemoteDevice) return false;
    
    Serial.print("Forming a connection to ");
    Serial.println(pRemoteDevice->getAddress().toString().c_str());
    
    pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());
    //17:11:33.982 -> Forming a connection to 44:17:93:5a:9d:3a
    //17:11:33.982 ->  - Created client
    // und dann passiert nix mehr... => fall fuer watchdog.
    // bekanntes problem? https://github.com/nkolban/esp32-snippets/issues/874
    //

    // Connect to the remove BLE Server.
    pClient->connect(pRemoteDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicStatus = pRemoteService->getCharacteristic(charUUIDstatus);
    if (pRemoteCharacteristicStatus == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUIDstatus.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    if(pRemoteCharacteristicStatus->canNotify()) {
      pRemoteCharacteristicStatus->registerForNotify(notifyCallback);
    } else {
      pClient->disconnect();
      return false;
    }
      
    Serial.println(" - canNotify ok");
    
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicCmd = pRemoteService->getCharacteristic(charUUIDcmd);
    if (pRemoteCharacteristicCmd == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUIDcmd.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    if(pRemoteCharacteristicCmd->canWrite()) {
      uint8_t str[] = "HELLO";
      pRemoteCharacteristicCmd->writeValue(str, sizeof(str));
//      std::string value = pRemoteCharacteristic->readValue();
//      Serial.print("The characteristic value was: ");
//      Serial.println(value.c_str());
    } else {
      pClient->disconnect();
      return false;
    }
    Serial.println(" - canWrite ok");

    nrBLEConnects++;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      scanActive = false;
      pRemoteDevice = new BLEAdvertisedDevice(advertisedDevice);

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

bool BLEwriteCMD(uint8_t* data) {
    size_t length = 4;
    Serial.println("BLEwriteCMD");
    if (pRemoteCharacteristicCmd) {
       pRemoteCharacteristicCmd->writeValue(data,length);
       return true;
    }
    return false;
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

void scanCompleteCB(BLEScanResults res) {
  scanActive=false;
  Serial.println("BLE Scan completed...");
}

void BLEstart() {
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  scanActive = true;
  pBLEScan->start(5, &scanCompleteCB, false);
  Serial.println("BLE Scan started...");
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

    BLEDevice::init("");
    BLEstart();
}

void loop()
{
  //alle paar sekunden probiere ggf. neu mit Wifi und BLE zu verbinden.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >=interval) {
    // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print(currentMillis);
      Serial.println("Reconnecting to WiFi...");
      WiFiConnect();
    }
  
    if (!scanActive && !pRemoteDevice) {
      BLEstart();
    }
    previousMillis = currentMillis;
  }
//  if (pRemoteCharacteristicStatus && currentMillis - tLastBLEStatusNotify >=interval) {
//    Serial.println("The BLE server seems connected but did not send notifies for too long..."); //probably the lld_pdu_get_tx_flush_nb HCI packet count mismatch thing...  did not work as expected so leave commented...
//    pClient->disconnect();
//    delay(300);
//  }
  
  if (pRemoteDevice && !pRemoteCharacteristicStatus) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
  }

  // now transfered to main loop for easier debugging
  if (bleCmdPending) {
    if (BLEwriteCMD(bleCmd))
      Serial.println("CMD written");
    else
      Serial.println("CMD write failed");
    bleCmdPending = false;
  }
  if (bleStatusNew) {
    sendUDPStatus();
    bleStatusNew = false;
  }

  digitalWrite(LED_BT,  pClient ? HIGH : LOW);
  digitalWrite(LED_WIFI, WiFi.status() == WL_CONNECTED ? HIGH : LOW);

  esp_task_wdt_reset();

  delay(10);
    
}



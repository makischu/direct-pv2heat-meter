//03.08.22 makischu as-is
//part of direct-pv2heat-meter. bridge between hardware (PCB) and bluetooth low energy (BLE).
// sends out status (voltage, current, ...) every second
// can receive a few commands (turing power output off/on, ...).
//
//makes use of esp32 board support for arduino and some arduino librarys: 
// Temperature-Sens:  https://github.com/milesburton/Arduino-Temperature-Control-Library
// Volt&Current-Sens: https://github.com/sparkfun/SparkFun_ACS37800_Power_Monitor_Arduino_Library
//and is partly based on the following code examples:
// Timer:             https://github.com/espressif/arduino-esp32/blob/master/libraries/ESP32/examples/Timer/RepeatTimer/RepeatTimer.ino
// BLE:               https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/examples/BLE_notify/BLE_notify.ino

/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
/*
  Library for the Allegro MicroSystems ACS37800 power monitor IC
  By: Paul Clark
  SparkFun Electronics
  Date: January 21st, 2021
  License: please see LICENSE.md for details
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17873
*/

#include "SparkFun_ACS37800_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_ACS37800
#include <Wire.h>

ACS37800 mySensor; //Create an object of the ACS37800 class


// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS (16)

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 Repeat timer example

 This example shows how to use hardware timer in ESP32. The timer calls onTimer
 function every second. The timer can be stopped with button attached to PIN 0
 (IO0).

 This example code is in the public domain.
 */
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
}


BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicStatus = NULL;
BLECharacteristic* pCharacteristicCmd = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                "58fad6cc-53b4-4dfd-ad3f-2072af1bbf2c"
#define CHARACTERISTIC_STATUS_UUID  "2a201432-a3c9-4b7c-b3ed-24b72d7f9158"
#define CHARACTERISTIC_CMD_UUID     "cf1ac9a7-87fe-400a-9bcf-a296dbc868c4"


float voltsAtLastRelaisCmd=0;
uint8_t lastExcecutedCmdNr=0;

uint32_t powerOutOn_BT=0;
uint32_t t_powerOutOn_BT=0;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

#define LED_BT        (2)
#define __PER_EN      (5)
#define PIN_IN1       (34)
#define PIN_IN2       (35)
#define PIN_OUT1      (32)
#define PIN_OUT2      (33)
#define PIN_OUT3      (25)
#define PIN_OUT4      (26)
#define FET2_ON       (PIN_OUT1)
#define FET3_ON       (PIN_OUT2)
#define RELAIS_SET    (PIN_OUT3)
#define RELAIS_RESET  (PIN_OUT4)

// set or reset the external relais.
void setRelais(bool doSet) {
    digitalWrite(FET2_ON, 0);
    digitalWrite(FET3_ON, 0);
    delay(400);
    digitalWrite(doSet ? RELAIS_SET : RELAIS_RESET, 1);
    delay(200);
    float volts = 0.0;
    float amps  = 0.0;
    float watts = 0.0;
    mySensor.readInstantaneous(&volts, &amps, &watts); 
    voltsAtLastRelaisCmd = volts;
    digitalWrite(doSet ? RELAIS_SET : RELAIS_RESET, 0);
}


void setup() {
  Serial.begin(115200);
  
  // initialize digital pin LED_BUILTIN as an output.
  Serial.println("Setting LED...");
  pinMode(LED_BT, OUTPUT);
  pinMode(__PER_EN, OUTPUT);
  pinMode(PIN_IN1, INPUT);
  pinMode(PIN_IN2, INPUT);
  pinMode(PIN_OUT1, OUTPUT);
  pinMode(PIN_OUT2, OUTPUT);
  pinMode(PIN_OUT3, OUTPUT);
  pinMode(PIN_OUT4, OUTPUT);
  
  // Start up the library
  sensors.begin();
  
  digitalWrite(LED_BT, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(__PER_EN, LOW);   // turn the LED off (HIGH is the voltage level)
  delay(1000);                       // wait for a second

 Wire.begin();

  //mySensor.enableDebugging(); // Uncomment this line to print useful debug messages to Serial

  //Initialize sensor using default I2C address
  if (mySensor.begin() == false)
  {
    Serial.print(F("ACS37800 not detected. Check connections and I2C address. Freezing..."));
    while (1)
      ; // Do nothing more
  }

  // From the ACS37800 datasheet:
  // CONFIGURING THE DEVICE FOR DC APPLICATIONS : FIXED SETTING OF N
  // Set bypass_n_en = 1. This setting disables the dynamic calculation of n based off voltage zero crossings
  // and sets n to a fixed value, which is set using EERPOM field n
  //
  // Sample rate is 32kHz. Maximum number of samples is 1023 (0x3FF) (10-bit)
  mySensor.setNumberOfSamples(1023, true); // Set the number of samples in shadow memory and eeprom
  mySensor.setBypassNenable(true, true); // Enable bypass_n in shadow memory and eeprom


  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristicStatus = pService->createCharacteristic(
                      CHARACTERISTIC_STATUS_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );
  pCharacteristicCmd = pService->createCharacteristic(
                      CHARACTERISTIC_CMD_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristicStatus->addDescriptor(new BLE2902());
  pCharacteristicCmd->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");


  
  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 1000000, true);

  // Start an alarm
  timerAlarmEnable(timer);
}

//unfortunately a function like this is missing in the library... other people seem to prefer float.
// returns temperature in 1/128 degrees C or DEVICE_DISCONNECTED_RAW if the
// device's scratch pad cannot be read successfully.
int32_t sensors_getTempRawByIndex(uint8_t deviceIndex) {
  DeviceAddress deviceAddress;
  if (!sensors.getAddress(deviceAddress, deviceIndex)) {
    return DEVICE_DISCONNECTED_RAW;
  }
  return sensors.getTemp((uint8_t*) deviceAddress);
}
//convert to 0.01°C, without float.
int16_t sensors_getTempIntcCByIndex(uint8_t deviceIndex) {
  int32_t tempRaw = sensors_getTempRawByIndex(deviceIndex);
  if (tempRaw <= DEVICE_DISCONNECTED_RAW) {
    return 0x8000;
  } else {
    //C = RAW/128 >= cC = RAW*100/128
    return (int16_t)((tempRaw*100)/128);
  }
}

void loop() {
  bool timerExpired = false;
  uint8_t bleCmd[4];
  uint8_t bleStatus[32]; uint8_t* pBleStatus=bleStatus;
  
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    uint32_t isrCount = 0, isrTime = 0;
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
    timerExpired = true;
//    // Print it
//    Serial.print("onTimer no. ");
//    Serial.print(isrCount);
//    Serial.print(" at ");
//    Serial.print(isrTime);
//    Serial.println(" ms");
  }

  if (timerExpired) {
    int in1,in2;
    float volts = 0.0;
    float amps  = 0.0;
    float watts = 0.0;
    float voltsRMS =0;
    float ampsRMS = 0;
    //float tempC[2];
    int16_t tempcC[2];
    uint16_t temp16;
    int16_t temp16s;
    
    in1 = digitalRead(PIN_IN1);
    in2 = digitalRead(PIN_IN2);

    mySensor.readInstantaneous(&volts, &amps, &watts); // Read the instantaneous voltage, current and power
    mySensor.readRMS(&voltsRMS, &ampsRMS); 
//    Serial.print(F("Volts: "));
//    Serial.print(volts, 2);
//    Serial.print(F(" Amps: "));
//    Serial.print(amps, 2);
//    Serial.print(F(" Watts: "));
//    Serial.println(watts, 2);
  
    sensors.requestTemperatures(); // Send the command to get temperatures
    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    //tempC[0] = sensors.getTempCByIndex(0);
    //tempC[1] = sensors.getTempCByIndex(1);
    tempcC[0] = sensors_getTempIntcCByIndex(0);
    tempcC[1] = sensors_getTempIntcCByIndex(1);
//    // Check if reading was successful
//    if(tempC != DEVICE_DISCONNECTED_C) 
//    {
//      //Serial.print("Temperature for the device 1 (index 0) is: ");
//      //Serial.println(tempC);
//    } 
//    else
//    {
//      //Serial.println("Error: Could not read temperature data");
//    }

    //Write Status...
    *(pBleStatus++) = lastExcecutedCmdNr;
    *(pBleStatus++) = (in1?0x01:0) | (in2?0x02:0);
    memcpy(pBleStatus,&(tempcC[0]),2); pBleStatus+=2;
    memcpy(pBleStatus,&(tempcC[1]),2); pBleStatus+=2;
    temp16 = (uint16_t)(volts*1000.0f);     memcpy(pBleStatus,&temp16,2); pBleStatus+=2;
    temp16s = (int16_t)(amps*1000.0f);      memcpy(pBleStatus,&temp16s,2);pBleStatus+=2;
    temp16 = (uint16_t)(voltsRMS*1000.0f);  memcpy(pBleStatus,&temp16,2);pBleStatus+=2;
    temp16s = (int16_t)(ampsRMS*1000.0f);   memcpy(pBleStatus,&temp16s,2);pBleStatus+=2;
    temp16 = (uint16_t)(voltsAtLastRelaisCmd*1000.0f);   memcpy(pBleStatus,&temp16,2);pBleStatus+=2;
    
    // notify changed value
    if (deviceConnected) {
        pCharacteristicStatus->setValue(bleStatus, pBleStatus-bleStatus);
        pCharacteristicStatus->notify();
        delay(9); // 99. bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }

    //handle command
   if (deviceConnected) {
        uint8_t* pRawCmd = pCharacteristicCmd->getData();
        uint8_t cmdNr = pRawCmd[0];
        uint8_t cmd   = pRawCmd[1];
        uint8_t cmdchk= pRawCmd[2];
        uint8_t cmd0  = pRawCmd[3];
        if (cmdNr != 0 && lastExcecutedCmdNr != cmdNr && cmdNr ^ cmd == cmdchk && cmd0 == 0) {
           digitalWrite(LED_BT, LOW);
           switch(cmd) {
           case 0xC0:
            powerOutOn_BT = 0;
            t_powerOutOn_BT = millis();
            delay(300);
             break;
           case 0xC1:
            powerOutOn_BT = 1;
            t_powerOutOn_BT = millis();
            delay(300);
             break;
           case 0xC5:
             setRelais(true);
             break;
           case 0xC8:
             setRelais(false);
             break;
           default:
           break;
           }
           lastExcecutedCmdNr = cmdNr;
        }
    }

    int fetsOn = in1 || (powerOutOn_BT && millis()<t_powerOutOn_BT+70000);
    digitalWrite(FET2_ON, fetsOn?HIGH:LOW);
    digitalWrite(FET3_ON, fetsOn?HIGH:LOW);

    digitalWrite(LED_BT, LOW);
    delay(100);
    digitalWrite(LED_BT, deviceConnected?HIGH:LOW);
  }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(200); // 500.give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

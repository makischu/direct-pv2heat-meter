//27.01.23 makischu as-is
//Version 04: for Hardware V04 - changed voltage supply and logic signals. includes uvlo latch and measurements.
//part of direct-pv2heat-meter. bridge between hardware (PCBs) and bluetooth low energy (BLE).
// sends out status (voltage, current, ...) every second
// can receive a few commands (turing power output off/on, ...).
// and switches between two wiring states automatically to maximize power output.
//
//makes use of esp32 board support for arduino and some arduino librarys: 
// Temperature-Sens:  https://github.com/milesburton/Arduino-Temperature-Control-Library
//and is partly based on the following code examples:
// Timer:             https://github.com/espressif/arduino-esp32/blob/master/libraries/ESP32/examples/Timer/RepeatTimer/RepeatTimer.ino
// BLE:               https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/examples/BLE_notify/BLE_notify.ino
// Shift register:    https://docs.arduino.cc/tutorials/communication/guide-to-shift-out#shftout11
// Firmwareupdate:    https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/OTAWebUpdater/OTAWebUpdater.ino
// ...


// PIN wiring
#define __PER_EN      (17)
#define __BUZZER      (18) // not placed.
#define LED_DATA      (23)
#define DIGIN1        (35)
#define DIGIN2        (34)
#define DIGIN3         (0)  
#define FET_STATUS    (27)
#define R_STATUS       (2)
#define FET_START     (25)
#define FET_EN        (26)
#define R_SET         (32)
#define R_RESET       (33)
#define ONE_WIRE_BUS  (16)
#define ANALOG_PIN24  (36)
#define ANALOG_PINS1  (39)

// output may only be enabled when ALL conditions are met.
#define OK_REQUEST    (0x01) //request must exist (hardwired / BLE / ext. temp sensor (the latter not implemented yet))
#define OK_RES_MON    (0x02) //load resistance has to be a senseful value
#define OK_TEMP_MON   (0x04) //internal temperature must not be too high
#define OK_NO_REBOOT  (0x08) //stop power when reboot requested
#define OK_HW         (0x20) //no hardware error must be known.

#define OK_ALL (OK_REQUEST|OK_RES_MON|OK_TEMP_MON|OK_NO_REBOOT|OK_HW)

//errors are assumed to be permanent hardware failures (maybe fixed with next restart/repowering)
#define ERR_ADC       (0x01)
#define ERR_1W        (0x02)
#define ERR_BADFET    (0x04)

//status bits (for output via BLE)
#define STA_RELAIS_SET            (0x01)
#define STA_FET_ON                (0x02)
#define STA_INFO                  (0x04)
#define STA_ERROR                 (0x08)
#define STA_DIGIN1                (0x10)
#define STA_DIGIN2                (0x20)
#define STA_DIGIN3                (0x40)
#define STA_DIGIN1_NOT            (0x80)
#define STA_RELAIS_SET_FORCED    (0x100)
#define STA_RELAIS_RESET_FORCED  (0x200)
#define STA_FAST_OFF_ACTION      (0x400)
//higher bits are used for temporary things


// each time step consists of 3 steps:
// 1. read inputs, from hardware and BLE.
// 2. interpret&act, incl. power output handling (the most interesting part).
// 3. generate and write (logic) outputs (LEDs and BLE).

// variables read from hardware
struct Inputs {
  float     UoutV;
  float     IoutA;
  uint16_t  U24mV;
  uint16_t  US1cV;
  int16_t   TempIntcC;
  int16_t   TempExtcC;
  int       DigIn1;
  int       DigIn2;
  int       DigIn3;
  int       RelaisStatus; //0 reset, 1 set.
  int       FetStatus; //0 off, 1 on.
  uint8_t   BleCmd[4];
};

// states which are calculated/stored internally, including power logic.
struct Internals {
  float     Inoload;
  float     Rload;
  uint32_t  t_Rload_set;
  float     PwrOutW;
  int       Errors;       //ERR_*
  int       PwrOutOk;     //OK_*
  uint8_t 	LastReceivedCmdNr;
  uint32_t  PowerOutOn_BT;
  uint32_t  t_PowerOutOn_BT;
  uint32_t  t_LastRelaisAction;
  int       relaisActionCount;
  int       RelaisMode;   // 5 set, 8 reset, other: automatic (->StateOut).
  uint16_t  U24mV_atLastRelaisCmd;  
  int       relaisActionCount_lastReported;
  int       fastAction;
  int       OTARequested;
  int       ResetRequested;
};

// outputs to be displayed and/or sent.
struct __attribute__((packed)) Outputs {
  uint16_t  Status;    //STA_...bits
  uint16_t  TempIntcC;
  uint16_t  TempExtcC;
  uint16_t  UoutcV;
  int16_t  IoutmA;
  uint16_t  U24mV;  // U24mV_atLastRelaisCmd at change of LastReceivedCmdNr
  uint16_t  US1cV;
  uint16_t  __free__was_PwrAlttW;
  uint16_t  __free__was_a_inv_em9; //inverted, times e+9.
  uint8_t   __free__was_b_di2_em3; //divided/2, times e+3.
  uint8_t   LastReceivedCmdNr;
}; //max 20bytes.

Inputs    input;
Internals intern;
Outputs   output;



// Math (see theory). 
// simplified and only valid for my configuration of source and load
inline void getSwitchingPointsP(float* pPtoSet, float* pPtoReset, float T_C) {
  const float cSp_par = -2.045406502f;
  const float Sp0_par = 326.6950852f;
  const float cSp_ser = -2.44131286f;
  const float Sp0_ser = 400.671502f;
  float PtoSer = Sp0_ser + cSp_ser*T_C - 5.0f;
  float PtoPar = Sp0_par + cSp_par*T_C;
  if(pPtoSet)   *pPtoSet = PtoSer;
  if(pPtoReset) *pPtoReset = PtoPar;
}
// simplified and only valid for my configuration of source and load
inline void getSwitchingPointsU(float* pUtoSet, float* pUtoReset, float T_C) {
  const float cSp_par = -0.36754060106676795f;
  const float Sp0_par = 107.56084823287758f;
  const float cSp_ser = -0.39504819148813525f;
  const float Sp0_ser = 119.10965758956989f;
  float UtoSer = Sp0_ser + cSp_ser*T_C;
  float UtoPar = Sp0_par + cSp_par*T_C;
  if(pUtoSet)   *pUtoSet = UtoSer;
  if(pUtoReset) *pUtoReset = UtoPar;
}


/////////////////////////////////////
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
const char* host = "esp32meter";
const char* ssid = "yourSSID";
const char* password = "yourPW";
WebServer server(80);
int OTAMode=0;
/*
 * Login page
 */
const char* loginIndex =
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32meter Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
             "<td>Username:</td>"
             "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";

const char* htmlReboot =
 "rebooting";
/*
 * Server Index Page
 */
const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";


/***************************************************************************
* Example sketch for the ADS1115_WE library
*
* This sketch shows how to use the ADS1115 in single shot mode. 
*  
* Further information can be found on:
* https://wolles-elektronikkiste.de/ads1115 (German)
* https://wolles-elektronikkiste.de/en/ads1115-a-d-converter-with-amplifier (English)
* 
***************************************************************************/

#include<ADS1115_WE.h> 
#include<Wire.h>
#define I2C_ADDRESS1 0x48
#define I2C_ADDRESS2 0x49

/* There are several ways to create your ADS1115_WE object:
 * ADS1115_WE adc = ADS1115_WE(); -> uses Wire / I2C Address = 0x48
 * ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS); -> uses Wire / I2C_ADDRESS
 * ADS1115_WE adc = ADS1115_WE(&Wire); -> you can pass any TwoWire object / I2C Address = 0x48
 * ADS1115_WE adc = ADS1115_WE(&Wire, I2C_ADDRESS); -> all together
 */
ADS1115_WE adc1 = ADS1115_WE(I2C_ADDRESS1);
ADS1115_WE adc2 = ADS1115_WE(I2C_ADDRESS2);

//read current through U1/ACS710/output
float readCurrent() {
  float voltage = 0.0;
  float current = 0.0;
  voltage = adc2.getResult_mV(); 
  current = -1.0*voltage/56.0*5.0/3.3;

  // print out the values you read:
  //Serial.printf("current out analog value = %f\n",voltage);
  //Serial.printf("Iout[V] = %f\n",current);

  return current;
}

//use the current current value as 0
void zeroCurrent() {
  float f=0;
  const int n = 10;
  //adc2.setConvRate(ADS1115_32_SPS);

  //self-calibration of zero current value
  delay(90); // let analog filter settle.
  f=0;
  for(int i=0;i<n;i++) { // some averaging is appropriate in case of noise
    delay(10); // samplerate 1/128 => 10ms should be sufficient to gain new value
    f += readCurrent(); 
  }
  f/=n;
  if (abs(f) < 1.0f) {
    intern.Inoload = f; 
  } else {
    Serial.print("inplausible no load current");
    intern.Errors |= ERR_BADFET;
  }
  
  //adc2.setConvRate(ADS1115_128_SPS);
}

float readCurrentCorrected() {
  float current = readCurrent();
  current -= intern.Inoload;
  return current;
}

//read voltage over output (before fets)
float readVoltageOut() {
  float voltage = 0.0;
  float strvolt = 0.0;
  voltage = adc1.getResult_mV(); 
  strvolt = -1.0*voltage/6.8*2006.8/1000.0;

  // print out the values you read:
  //Serial.printf("voltage out analog value = %f\n",voltage);
  //Serial.printf("Vout[V] = %f\n",strvolt);

  return strvolt;
}

//read voltage on 24V rail in mV
uint16_t readVoltage24mV() {
  //int analogValue24 = analogRead(ANALOG_PIN24);
  int analogVolts24 = analogReadMilliVolts(ANALOG_PIN24);
  int volts24 = analogVolts24 * ((470+10)/(10));
  
  // print out the values you read:
  // Serial.printf("24 ADC analog value = %d\n",analogValue24);
  // Serial.printf("24 ADC millivolts value = %d\n",analogVolts24);
  //Serial.printf("V24[mV] = %d\n",volts24);
  return (uint16_t)volts24;
}

//read voltage over S1 in cV
uint16_t readVoltageS1cV() {
  //int analogValueS1 = analogRead(ANALOG_PINS1);
  int analogVoltsS1 = analogReadMilliVolts(ANALOG_PINS1);
  int voltsS1 = analogVoltsS1 * ((20000+133)/(133));
  
  // print out the values you read:
  //Serial.printf("S1 ADC analog value = %d\n",analogValueS1);
  // Serial.printf("S1 ADC millivolts value = %d\n",analogVoltsS1);
  //Serial.printf("VS1[mV] = %d\n",voltsS1);
  return (uint16_t)(voltsS1/10);
}

//read DIGIN, by ADC to distinguish n.c.(-1), 0 and 1.
int readDigIn1() {
  int analogMilliVolts = analogReadMilliVolts(DIGIN1);
  int result = analogMilliVolts < 500 ? 0 : analogMilliVolts > 2000 ? 1 : -1;
  return result;
}
//read DIGIN, by ADC to distinguish n.c.(-1), 0 and 1.
int readDigIn2() {
  //int analogValue = analogRead(DIGIN2);
  int analogMilliVolts = analogReadMilliVolts(DIGIN2);
  int result = analogMilliVolts < 500 ? 0 : analogMilliVolts > 2000 ? 1 : -1;
  //Serial.printf("DigIn2 ADC analog value = %d\n",analogValue);
  //Serial.printf("DigIn2 ADC millivolts value = %d\n",analogMilliVolts);
  return result;
}



//**************************************************************//
//  Name    : shiftOutCode, Hello World
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis
//  Date    : 25 Oct, 2006
//  Modified: 23 Mar 2010
//  Version : 2.0
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255
//****************************************************************
//Pin connected to ST_CP of 74HC595
const int latchPin = 4;
//Pin connected to SH_CP of 74HC595
const int clockPin = 5;
//Pin connected to DS of 74HC595
const int dataPin = 19;


//https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
#include <Preferences.h>
Preferences preferences;
#define PREF_NS "esp32meter"
#define PREF_1WINT "1Wint"
#define PREF_OTARQ "OTARq"


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


// Include the libraries we need for 1Wire
#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//address of the internal temperature sensor. to distinguish from external on same bus.
uint8_t internalTempAddress[8];
uint8_t internalTempAddressOld[8];





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


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


//Illuminat status LEDs ("Outgoing User Interface"), except data LED.
void setRegisterLeds(int powerIndex, bool info, bool error, bool parallel, bool serial) 
{
  int byte1=0;
  int byte2=0;
  int byte3=0;
  if (powerIndex>=0 && powerIndex<=7) {
    byte1 |= 1<<(powerIndex);
  }
  if (powerIndex>=8 && powerIndex<=15) {
    byte2 |= 1<<(powerIndex-8);
  }
  if (powerIndex>=16 && powerIndex<=19) {
    byte3 |= 1<<(powerIndex-16);
  }
  if (info) {
    byte3 |= 0x10;
  }
  if (error) {
    byte3 |= 0x20;
  }
  if (parallel) {
    byte3 |= 0x80;
  }
  if (serial) {
    byte3 |= 0x40;
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, byte3);
  shiftOut(dataPin, clockPin, MSBFIRST, byte2);
  shiftOut(dataPin, clockPin, MSBFIRST, byte1);
  digitalWrite(latchPin, HIGH);
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

//read internal temperature, in 0.01°C.
int16_t sensors_getTempcCInternal() {
  int32_t tempRaw =  sensors.getTemp(internalTempAddress);
  if (tempRaw <= DEVICE_DISCONNECTED_RAW) {
    return 0x8000;
  } else {
    //C = RAW/128 >= cC = RAW*100/128
    return (int16_t)((tempRaw*100)/128);
  }
}

//read external (not the internal) temperature, if any.
int16_t sensors_getTempcCExternal() {
  uint8_t extIdx=0;
  DeviceAddress deviceAddress;
  if (!sensors.getAddress(deviceAddress, 0)) {
    return DEVICE_DISCONNECTED_RAW;
  }
  if(0==memcmp(deviceAddress,internalTempAddress,8)) {
    //wenn 0 der interne ist dann kann es nicht der externe sein => probiere 1.
    //wenn 0 nicht der interne ist, dann muss es der externe sein.
    extIdx=1;
  }
  return sensors_getTempIntcCByIndex(extIdx);
}



// Activate or deactivate FET.
void setFet(bool doSet) {
  if (doSet) {
    digitalWrite(FET_START, 1);
    delay(1);
    digitalWrite(FET_START, 0);
    delay(1);
  }
  digitalWrite(FET_EN, doSet?1:0);
}

#define ADC_SETTLING_TIME_MS (50+20)
// according to simulation, value decreases to <1% after 50ms, despite low pass filter. 
// consider adc sampling time. todo! solang getriggert ists egal, aber das will ich ja nicht dauerhaft.

// set or reset the power relais.
int setRelais(bool doSet, float* pUOff=0, float* pIOff=0) {
  uint16_t U24; float U; float I; int err=0;
  //ensure that no current flows...
  setFet(0);
  delay(ADC_SETTLING_TIME_MS);
  I = readCurrentCorrected();
  U = readVoltageOut();
  if(pUOff) *pUOff=U;
  if(pIOff) *pIOff=I;
  // no current is allowed when switching the relais. 
  // dc breaking ability decreases a lot with current. see relais datasheet.
  // in doubt, do not operate.
  if(abs(I) > 0.3) {
    intern.U24mV_atLastRelaisCmd = 50000;
    intern.Errors |= ERR_BADFET;
    return -2;
  }
  zeroCurrent(); // offset seems to vary, maybe with temperature, so try recalibration every time the current is 0 by definition.
  //relais work
  digitalWrite(doSet ? R_SET : R_RESET, 1);
  delay(200);
  U24 = readVoltage24mV();
  digitalWrite(doSet ? R_SET : R_RESET, 0);
  intern.U24mV_atLastRelaisCmd = U24;
  intern.relaisActionCount++;
  intern.t_LastRelaisAction = millis();
  return err;
}

int readFetStatus() { 
  int res = digitalRead(FET_STATUS);
  return res;
}

// loop part 1
void readInputs(){
  //I2C ADC
  input.IoutA = readCurrentCorrected();
  input.UoutV = readVoltageOut();
  //internal ADC
  input.US1cV = readVoltageS1cV();
  input.U24mV = readVoltage24mV();
  // internal GPIO
  input.DigIn1 = readDigIn1();
  input.DigIn2 = readDigIn2();
  input.DigIn3 = digitalRead(DIGIN3);
  // relais feedback
  input.RelaisStatus = digitalRead(R_STATUS);
  // fet status
  input.FetStatus = readFetStatus();
  // 1W
  sensors.requestTemperatures(); // Send the command to get temperatures
  input.TempIntcC = sensors_getTempcCInternal();
  input.TempExtcC = sensors_getTempcCExternal();
}

// loop part 2
void interpretAndAct() {
  float U,I,R,P;
  bool Iexists;
  int err;
  float T=0;
  int PwrOutOk = 0;
  bool UVLOtriggered=false;
  int fetsRequested=0;
  float UtoSet=0,UtoReset=0;

  //another option would be: use external temperature sensor as requestor (if temp < threshold).
  fetsRequested = input.DigIn1==1 || (intern.PowerOutOn_BT && millis()<intern.t_PowerOutOn_BT+70000);
  if(fetsRequested) PwrOutOk |= OK_REQUEST;

  U = input.UoutV;
  I = input.IoutA;// already corrected
  Iexists = I>0.2;//heuristic, zero + offset + noise, avoid div0 

  if(input.TempExtcC != DEVICE_DISCONNECTED_RAW ) {
    T = input.TempExtcC/100.0f;
    getSwitchingPointsU(&UtoSet, &UtoReset, T); // ask the main "intelligence"
  }

  // Current Power
  P = U * I;
  intern.PwrOutW = P>0 ? P : 0;
  
  //UVLO monitoring
  // XJNG2103 UVLO is 5-5.5V. VGth IRFB4332PbF is 3-5V. 
  UVLOtriggered = intern.PwrOutOk == OK_ALL && input.FetStatus == 0;

  //Resistance Monitoring
  if (Iexists && !UVLOtriggered) {
    R = U/I;
    intern.Rload = R;
    intern.t_Rload_set = millis();
    //no real R monitoring implemented yet.
  }
  PwrOutOk |= OK_RES_MON;

  //Temperature Monitoring
  if (input.TempIntcC <= 5000) { // if it becomes hotter than 40°C at the sensor better turn off. leider wurden 40grad schnell erreicht, und zwar nicht vom fet sondern der power supply, da hilft das e nix.
    PwrOutOk |= OK_TEMP_MON;
  }

  if (intern.ResetRequested == 0) {
    PwrOutOk |= OK_NO_REBOOT;
  }

  // general hardware erro monitor.
  if (intern.Errors == 0) {
    PwrOutOk |= OK_HW;
  }

  
  //handle command
  uint8_t cmdNr = input.BleCmd[0];
  uint8_t cmd   = input.BleCmd[1];
  uint8_t cmdchk= input.BleCmd[2];
  uint8_t cmd0  = input.BleCmd[3];
  if (cmdNr != 0 && intern.LastReceivedCmdNr != cmdNr && cmdNr ^ cmd == cmdchk && cmd0 == 0) {
    intern.LastReceivedCmdNr  = cmdNr;
    switch(cmd) {
    case 0xC0:
      intern.PowerOutOn_BT = 0;
      intern.t_PowerOutOn_BT = millis();
      break;
    case 0xC1:
      intern.PowerOutOn_BT = 1;
      intern.t_PowerOutOn_BT = millis();
      break;
   case 0xC5:
      intern.RelaisMode = 5;
      break;
   case 0xC8:
      intern.RelaisMode = 8;
      break;
   case 0xCA:
      intern.RelaisMode = 0xA;
      break;
   case 0xCB:
      intern.ResetRequested = 1;
      break;
   case 0xCF:
      intern.OTARequested = 1;
      intern.ResetRequested = 1;
      break;
    default:
      break;
    }
  }


  // Relais bedienen
  err = 0;
  if (intern.RelaisMode == 5) {
    //relais set, fix.
    if (input.RelaisStatus != 1) {
      err = setRelais(1);
    }
  }
  else if (intern.RelaisMode == 8) {
    // relais reset, fix
    if (input.RelaisStatus != 0) {
      err = setRelais(0);
    }
  }
  else {
    // if UVLO triggered and we are in series mode, switch to parallel mode.
    if (UVLOtriggered && input.RelaisStatus == 1) {
      err = setRelais(0);
    }
    else if ((millis()-intern.t_LastRelaisAction)>2500 && input.FetStatus==1) {
      if (input.RelaisStatus == 0 && input.UoutV > UtoSet) { //intern.PwrOutW > PtoSet) {
        err = setRelais(1);
      }else if (input.RelaisStatus == 1 && input.UoutV < UtoReset) { //intern.PwrOutW < PtoReset) {
        err = setRelais(0);
      }
    }
  }
  
  // FETs on or off
  intern.PwrOutOk = PwrOutOk;
  setFet(intern.PwrOutOk == OK_ALL ? 1 : 0);

  //Reboot & OTA-Reboot
  if (intern.ResetRequested) {
    // give a chance to report the command
    if (intern.ResetRequested>=3) {
      if (intern.OTARequested) {
        preferences.begin(PREF_NS, false); 
        preferences.putBool(PREF_OTARQ, true);
        preferences.end();
        Serial.println("OTA requested  - reboot");
      }

      ESP.restart();
    }
    intern.ResetRequested++;
  }

  //tend = millis();
  //Serial.print("dt=");
  //Serial.println(tend-tstart);
}

// loop part 3
void generateOutputs() {
  uint16_t status=0; float ftmp; 
  if (input.RelaisStatus)       status |= STA_RELAIS_SET;
  if (input.FetStatus)          status |= STA_FET_ON; 
  if (intern.PwrOutOk!=OK_ALL)  status |= STA_INFO;
  if (intern.Errors)            status |= STA_ERROR; //evtl besser auflösen?
  if (input.DigIn1==1)          status |= STA_DIGIN1;
  if (input.DigIn2==1)          status |= STA_DIGIN2;
  if (input.DigIn3==1)          status |= STA_DIGIN3;
  if (input.DigIn1==0)          status |= STA_DIGIN1_NOT;
  if (intern.RelaisMode==0x05)  status |= STA_RELAIS_SET_FORCED;
  if (intern.RelaisMode==0x08)  status |= STA_RELAIS_RESET_FORCED;
  if (intern.fastAction)        status |= STA_FAST_OFF_ACTION;
  output.Status = status;
  output.TempIntcC = input.TempIntcC;
  output.TempExtcC = input.TempExtcC;
  output.UoutcV = (uint16_t)(input.UoutV*100);
  output.IoutmA = (int16_t)(input.IoutA*1000);
  output.U24mV  = input.U24mV;
  output.US1cV  = input.US1cV;
  if (intern.relaisActionCount != intern.relaisActionCount_lastReported) { 
    output.U24mV = intern.U24mV_atLastRelaisCmd;
    intern.relaisActionCount_lastReported = intern.relaisActionCount;
  }
  output.LastReceivedCmdNr     = intern.LastReceivedCmdNr;
  intern.fastAction = 0;
}

//called every 100ms, for specific functions to act faster.
void actFast() {
  float U,I,R,fac;
  uint32_t t = millis();
  I = readCurrentCorrected();
  U = readVoltageOut();
  if (I > 0.05 && intern.Rload > 0.05 && (t-intern.t_Rload_set) < 1000) {
    R = U/I;
    fac = R/intern.Rload;
    if (fac > 1.1 || fac < 0.9) { // inplausible load change, better stop
      setFet(0);
      intern.fastAction = 1;
    }
  }
}


// setup code, to run once:
void setup() {
  bool OTArequested = false;  int digIn1;
  memset(&input, 0, sizeof(Inputs));
  memset(&intern, 0, sizeof(Internals));
  memset(&output, 0, sizeof(Outputs));

  pinMode(__PER_EN, OUTPUT);  digitalWrite(__PER_EN, HIGH); 
  pinMode(latchPin, OUTPUT);  digitalWrite(latchPin, HIGH); 
  pinMode(clockPin, OUTPUT);  digitalWrite(clockPin, HIGH); 
  pinMode(dataPin, OUTPUT);   digitalWrite(dataPin,  LOW); 
  pinMode(__BUZZER, OUTPUT);  digitalWrite(__BUZZER, HIGH); 
  pinMode(LED_DATA, OUTPUT);  digitalWrite(LED_DATA, LOW); 
  pinMode(R_SET, OUTPUT);     digitalWrite(R_SET, LOW);
  pinMode(R_RESET, OUTPUT);   digitalWrite(R_RESET, LOW); 
  pinMode(FET_EN, OUTPUT);    digitalWrite(FET_EN, LOW); 
  pinMode(FET_START, OUTPUT); digitalWrite(FET_START, LOW);
 
  pinMode(DIGIN1, INPUT);
  pinMode(DIGIN2, INPUT);
  pinMode(DIGIN3, INPUT);
  pinMode(R_STATUS, INPUT); 
  pinMode(FET_STATUS, INPUT); 

  Serial.begin(115200);
  Serial.print("getXtalFrequencyMhz: ");  Serial.println(getXtalFrequencyMhz());
  Serial.print("getCpuFrequencyMhz:  ");  Serial.println(getCpuFrequencyMhz());
  Serial.print("getApbFrequency:     ");  Serial.println(getApbFrequency());

  //I manually connected R20 to buzzer-out instead of V33per. should be V33 in case of redesign.
  digitalWrite(__BUZZER, LOW); 
  delay(10);
  
  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
  adcAttachPin(ANALOG_PIN24);
  analogSetPinAttenuation(ANALOG_PIN24, ADC_0db);
  adcAttachPin(ANALOG_PINS1);
  analogSetPinAttenuation(ANALOG_PINS1, ADC_0db);
  adcAttachPin(DIGIN1);
  analogSetPinAttenuation(DIGIN1, ADC_11db);
  adcAttachPin(DIGIN2);
  analogSetPinAttenuation(DIGIN2, ADC_11db);
  delay(10);
  digIn1 = readDigIn1();

  //OTA
  preferences.begin(PREF_NS, true); 
  OTArequested = preferences.getBool(PREF_OTARQ, false);
  preferences.end();
  if (OTArequested) {
    preferences.begin(PREF_NS, false); 
    preferences.putBool(PREF_OTARQ, false);
    preferences.end();
  }
  OTAMode = OTArequested || digIn1!=-1;
  if (OTAMode) {
    Serial.println("OTAMode");
    // Connect to WiFi network
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    digitalWrite(LED_DATA, HIGH);
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    /*use mdns for host name resolution*/
    if (!MDNS.begin(host)) { //http://esp32.local
      Serial.println("Error setting up MDNS responder!");
      while (1) {
        delay(1000);
      }
    }
    Serial.println("mDNS responder started");
    /*return index page which is stored in serverIndex */
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", loginIndex);
    });
    server.on("/serverIndex", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/reboot", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", htmlReboot);
      ESP.restart();
    });
    /*handling uploading firmware file */
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
    server.begin();
    return;
  }
  
  delay(10);
  setRegisterLeds(-1, false, false, false, false);

  delay(890);
  digitalWrite(__PER_EN, LOW); 
  delay(100);


  // Start up the ADC library
  Wire.begin();
  if(!adc1.init()){
    Serial.println("ADS1115 1 not connected!");
    intern.Errors |= ERR_ADC;
  }
  if(!adc2.init()){
    Serial.println("ADS1115 2 not connected!");
    intern.Errors |= ERR_ADC;
  }
  adc1.setCompareChannels(ADS1115_COMP_0_1);
  adc1.setVoltageRange_mV(ADS1115_RANGE_1024); 
  adc1.setConvRate(ADS1115_128_SPS);
  adc1.setMeasureMode(ADS1115_CONTINUOUS);
  adc2.setCompareChannels(ADS1115_COMP_0_1);
  adc2.setVoltageRange_mV(ADS1115_RANGE_1024); 
  adc2.setConvRate(ADS1115_128_SPS);
  adc2.setMeasureMode(ADS1115_CONTINUOUS);

  zeroCurrent();

  Serial.print("sizeof(output)=");
  Serial.println(sizeof(output)); // limited to xx bytes due to BLE

  // Start up the 1W library
  sensors.begin();
  if (0==sensors.getDeviceCount()) {
    sensors.begin();
  }

  //check if at least 1 sensor is connected. the internal one must be there.
  if (0==sensors.getDeviceCount()) {
    Serial.println("DS18B20 not connected!");
    intern.Errors |= ERR_1W;
  }

  //read the internal 1w sensor (if stored. if not, try to store.)
  preferences.begin(PREF_NS, true); 
  memset(internalTempAddress,0,8);
  memset(internalTempAddressOld,0,8);
  if (8!=preferences.getBytes(PREF_1WINT, internalTempAddress, 8)) {
    // wenn noch keiner gespeichert ist, versuche den internen zu speichern
    // dazu definiere ich dass wenn nur 1 dran ist, das der interne ist
    // dazu muss also nur beim einrichten 1x ohne externen sensor eingeschaltet werden.
    if (1==sensors.getDeviceCount()) {
      if (sensors.getAddress(internalTempAddress,0)) {
        preferences.end();
        preferences.begin(PREF_NS, false); 
        preferences.putBytes(PREF_1WINT, internalTempAddress, 8);
          Serial.println("storing internal temp sensor address");
      }
    }
  }
  //mit der obigen lösung kann ich die platinen aber nicht untereinander tauschen,
  // sonst hält er den internen für den externen und findet den internen nicht.
  if (8==preferences.getBytes(PREF_1WINT, internalTempAddressOld, 8)) {  
    if (1==sensors.getDeviceCount()) {
      if (sensors.getAddress(internalTempAddress,0)) {
        if (memcmp(internalTempAddress, internalTempAddressOld, 8) != 0) {
          preferences.end();
          preferences.begin(PREF_NS, false); 
          preferences.putBytes(PREF_1WINT, internalTempAddress, 8);
          Serial.println("updating internal temp sensor address");
        }
      }
    }    
  }
  preferences.end();

  // Create the BLE Device
  BLEDevice::init("ESP32pv");

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
  //timerAlarmWrite(timer, 1000000, true);
  timerAlarmWrite(timer, 100000, true); //every 100ms | 10 times/second  

  // Start an alarm
  timerAlarmEnable(timer);
}

// main code, to run repeatedly:
void loop() {
  uint32_t isrCount = 0, isrTime = 0;
  bool timer100msExpired = false;
  bool timer1000msExpired = false;
  
  //OTA
  if(OTAMode) {
   server.handleClient();
   delay(1);
   return;
  }
  
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
    timer1000msExpired = isrCount%10==0;
    timer100msExpired = !timer1000msExpired;
    // if (timer1000msExpired) {
    //   // Print it
    //   Serial.print("onTimer no. ");
    //   Serial.print(isrCount);
    //   Serial.print(" at ");
    //   Serial.print(isrTime);
    //   Serial.print(" ms / ");
    //   Serial.print(millis());
    //   Serial.println(" ms");
    // }
  }

  if(timer100msExpired) {
    actFast();
  }
  if (timer1000msExpired) {
    //read BLE input
    if (deviceConnected) {
        uint8_t* pRawCmd = pCharacteristicCmd->getData();
        if(pRawCmd) memcpy(input.BleCmd, pRawCmd, 4);
    } else {
       memset(input.BleCmd, 0, 4);
    }

    ////////////////////////////////////////////////////vvvv MAIN LOGIC vvvv
    //Serial.println("_");
    readInputs();
    //Serial.println("__");
    interpretAndAct();
    //Serial.println("___");
    generateOutputs();
    //Serial.println("____");
    ////////////////////////////////////////////////////^^^^ MAIN LOGIC ^^^^

    //Set static leds
    int pwrIdx = ( (int)output.UoutcV * (int)output.IoutmA ) / (100 * 100* 1000);
    pwrIdx = min(pwrIdx, 19);
    if (input.FetStatus==0) pwrIdx = -1;
    setRegisterLeds(pwrIdx, output.Status&STA_INFO, output.Status&STA_ERROR, input.RelaisStatus==0, input.RelaisStatus==1);
   
    // notify changed value on BLE
    if (deviceConnected) {
        pCharacteristicStatus->setValue((uint8_t*)&output, sizeof(output));
        pCharacteristicStatus->notify();
        delay(9); // 99. bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }

    //set dynamic led.
    digitalWrite(LED_DATA, LOW);
    delay(100);
    digitalWrite(LED_DATA, deviceConnected?HIGH:LOW);
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

  delay(1);
}


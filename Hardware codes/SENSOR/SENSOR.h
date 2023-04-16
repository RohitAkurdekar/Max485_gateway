/********************************************************************************************
 * Name:- MODBUS MASTER HEADER file
 * Date:- 20/Feb/2023
 * Developer:- Rohit Akurdekar
 * Brief:- 
 SENSOR:
     * Read data from BMP280 sensor.
 MODBUS:
     * Send data over MODBUS.
 * Data - JSON format
 * Note:- Verify libraries and change Network credentials.
*********************************************************************************************/

#ifndef SENSOR_H
#define SENSOR_H

//---------------------------------- Libraries inclusion -----------------------------------

#include <Wire.h>
#include <Adafruit_BMP280.h>

#include "Arduino.h"
#include "HardwareSerial.h"

#include <ArduinoJson.h>


// #include "Preferences.h"

#include <WString.h>
// ------------- CONSTANTS ------------------------
#define DEVICE_ID "BME280_1"
// String DEVICE_ID = "BME280_1";
#define DEVICE_NO "Device1"

#define BAUDRATE 19200

// #define ARDUINOJSON_USE_DOUBLE 1

// ------------- CONSTRUCTORS ---------------------

Adafruit_BMP280 bmp; // I2C


DynamicJsonDocument JSON(512);

// --------------- variables ---------------------------
int enablePin = 2;          // Direction control pin
int potval =0 ;

// float temperature ;
// float pressure    ;
// float altitude    ;
// ------------- Functions -------------------------

/********************************************************************************
 * @name    : void vEnableSensor()
 * @param   : None
 * @returns : None
 * @date    : 20 Feb 2023
 * @brief   : Configure MODBUS and BMP280 sensor.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vEnableSensor()
{
  // ---------- MODBUS ----------------------
  Serial.begin(BAUDRATE);            // initialize serial at baudrate 19200:

  pinMode(enablePin, OUTPUT);

  delay(10); 

  digitalWrite(enablePin, HIGH);  //  (always high as Master Writes data to Slave)


  // ------- BME280 ----------------------
  unsigned status;
  status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


};

/********************************************************************************
 * @name    : void vReadData()
 * @param   : None
 * @returns : None
 * @date    : 20 Feb 2023
 * @brief   : Read temperature, pressure and altitude from BMP280.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vReadData()
{
  // temperature = bmp.readTemperature();
  // pressure    = bmp.readPressure();
  // altitude    = bmp.readAltitude(1013.25); 
}

/********************************************************************************
 * @name    : void vSendData()
 * @param   : None
 * @returns : None
 * @date    : 20 Feb 2023
 * @brief   : Send temperature, pressure and altitude in JSON format.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vSendData()
{

  // vReadData();
  String rxdData;

  digitalWrite(enablePin, LOW);
  delay(50);
  // while(Serial.available() == 0){}

  rxdData = Serial.readStringUntil('#');
  // Serial.println("rcd"+rxdData);
 
  if(rxdData.length() == 0)
  {
     rxdData = Serial.readStringUntil('#');
  }
  // Serial.println(rxdData);
  int pos = rxdData.indexOf('@');
  rxdData = rxdData.substring(pos+1);

  delay(500);

  if(rxdData == DEVICE_NO)
  {
    digitalWrite(enablePin, HIGH);
    delay(30);
  
    String tempData ;
  
    float temperature = bmp.readTemperature();
    float pressure    = bmp.readPressure();
    float altitude    = bmp.readAltitude(1013.25); 


    JSON["DEVICE_ID"] = DEVICE_ID;
    JSON["TEMPERATURE"] = 32.2;
    JSON["PRESSURE"] = pressure;
    JSON["ALTITUDE"] = altitude;

    serializeJson(JSON,tempData);     // JSON stringify | JSON dumps
    delay(10);

    // Serial.println(tempData);

    String data = (String)"&" +(String)tempData + (String)"$";

    Serial.println(data);
    Serial.flush();
    // Serial.print("Alti: ");
    // String tmp_as = JSON["DEVICE_ID"];

    // Serial.println(tmp_as);

    // Serial.println(tempData);

    // String data = "&{\"DEVICE_ID\":"+(String)DEVICE_ID+",\"temperature\":"+(String)temperature + ",\"pressure\":"+ (String)pressure +",\"altitude\":"+ (String)altitude + "}$";

    delay(100);
    JSON.clear();
  }

  

  delay(100);

}

// -------------------- EoF -----------------------------------------------------------------

#endif
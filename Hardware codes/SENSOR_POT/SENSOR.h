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
#include "Arduino.h"
#include "HardwareSerial.h"

#include <ArduinoJson.h>

DynamicJsonDocument JSON(1024);

// ------------- CONSTANTS ------------------------
#define DEVICE_ID "POT_1"
#define DEVICE_NO "Device2"

#define BAUDRATE 19200

// #define VCC_PIN 13      // VCC for test purpose
#define AIN 35

// --------------- variables ---------------------------
int enablePin = 22;          // Direction control pin
int potval =0 ;

float sensor_value;
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

  // pinMode(VCC_PIN, OUTPUT);
  // digitalWrite(VCC_PIN, HIGH);


};

/********************************************************************************
 * @name    : void vReadData()
 * @param   : None
 * @returns : None
 * @date    : 20 Feb 2023
 * @brief   : Read value from single wire sensor.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vReadData()
{
  sensor_value = analogRead(AIN);
}

/********************************************************************************
 * @name    : void vSendData()
 * @param   : None
 * @returns : None
 * @date    : 20 Feb 2023
 * @brief   : Send value in JSON format.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vSendData()
{

  digitalWrite(enablePin, LOW);
  delay(50);
  while(Serial.available() == 0){}

  String rxdData = Serial.readStringUntil('#');

  if(rxdData.isEmpty())
  {
    rxdData = Serial.readStringUntil('#');
  }
  if(rxdData.isEmpty())
  {
    rxdData = Serial.readStringUntil('#');
  }  
  
  // Serial.println("rcd: "+rxdData);
  int pos = rxdData.indexOf('@');
  rxdData = rxdData.substring(pos+1);
  // Serial.println("rcd: "+rxdData);

  // delay(500);
  
  if(rxdData == DEVICE_NO)
  {
    // Serial.println("trueee");
    digitalWrite(enablePin, HIGH);
    delay(30);
    
    String data, tempData;

    JSON["DEVICE_ID"] = DEVICE_ID;
    JSON["VALUE"] = sensor_value;

    serializeJson(JSON,tempData);     // JSON stringify | JSON dumps

    data = "&" + tempData + "$";

    // String data = "&{\"DEVICE_ID\":"+(String)DEVICE_ID+",\"value\":"+(String)sensor_value + "}$";
    Serial.println(data);
    Serial.flush();
  }

  delay(100);

}

// -------------------- EoF -----------------------------------------------------------------

#endif
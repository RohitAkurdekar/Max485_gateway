/********************************************************************************************
 * Name:- MODBUS MASTER 
 * Date:- 20/Feb/2023
 * Developer:- Rohit Akurdekar
 * Brief:- 
 SENSOR:
     * Read data from BMP280 sensor.
 MODBUS:
     * Send data over MODBUS
 * Data - JSON format
 * Note:- Verify libraries and change Network credentials.
*********************************************************************************************/

#include "SENSOR.h"

void setup()

{

  vEnableSensor();    // Enable Sensor
  
}

void loop()
{
  
  vReadData();
  vSendData();

}

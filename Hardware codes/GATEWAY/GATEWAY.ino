/********************************************************************************************
 * Name:- MODBUS SLAVE 
 * Date:- 20/Feb/2023
 * Developer:- Rohit Akurdekar
 * Brief:- 
 MODBUS slave
     * READ data over MODBUS
 MQTT publisher functions.
     * Connect to Network
     * Connect to MQTT broker'
     * Publish message over MQTT
 * Note:- Verify libraries and change Network credentials.
*********************************************************************************************/

#include"GATEWAY.h"


// -----------ś------ SETUP -----------------------------
void setup() 
{
  vInitEEPROM();

 /**
  epr32.clear();  // clear all eprom 
  Serial.println("");
  Serial.println("EEPROM cleared.");
  while (1)
  {
      //Uncomment this section to clear EEPROM and comment it again after upload 
  }/**/
  
  vFirstTimeWrite();
  vEepromRead();

  WiFi.mode(WIFI_AP_STA);
  vEnableGateway();

  vBeginSoftAP();
  vServer();

  isWiFiBegin();

  client.setServer(MQTT_BROKER, 1883);      // Connect to MQTT broker
  client.setCallback(subscribe);

  vWDTInit();

}

// ------------------------- LOOP -----------------------------------
void loop() 
{
  if (!client.connected()) 
  {
    vReconnect();
  }
  client.loop();

  vSendData();
  // digitalWrite(ledpin, HIGH);

  vWDT();

  delay(2000);

}

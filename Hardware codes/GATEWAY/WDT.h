#ifndef WDT_H
#define WDT_H

//---------------------------------- Libraries inclusion -----------------------------------

#include <esp_task_wdt.h>

#include<esp_err.h>

//---------------------------------- CONSTANTS --------------------------------------------

#define WDT_TIMEOUT 30           // Watch dog timer

//---------------------------------- variables --------------------------------------------
int last = millis();


/***************************************************************************************
* Subroutine : void vWDTInit(void)
* Tested     :  
* Created on :
* Version    :
* Parameters : 
* Description: Initialize WDT
* Note       : 
***************************************************************************************/
void vWDTInit(void)
{
  //esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_init(WDT_TIMEOUT,true);
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}
/***************************************************************************************
* Subroutine : void vWDTInit(void)
* Tested     : OK  
* Created on : 24 Feb 2023
* Version    : 1
* Parameters : None
* Description: Reset WDT and ESP
* Note       : 
***************************************************************************************/
void vWDT(void)
{
  // resetting WDT every 2s, 5 times only
  if (millis() - last >= 2000  ) {
      // Serial.println("Resetting WDT...");
      if (esp_task_wdt_reset() != ESP_OK) {
        Serial.println("Stopping WDT reset. CPU should reboot in " +(String)WDT_TIMEOUT + "s");
      }
      last = millis();
     /* i++;
      if (i == 5) {
        Serial.println("Stopping WDT reset. CPU should reboot in" +(String)WDT_TIMEOUT + "s");
      }*/
  }
  
}

// -------------------------------------- EoF ----------------------------------------------
#endif
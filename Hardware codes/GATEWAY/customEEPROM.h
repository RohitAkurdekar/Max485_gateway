#ifndef customEEPROM_H
#define customEEPROM_H

//---------------------------------- Libraries inclusion -----------------------------------

#include "Preferences.h"
#include "string.h"


Preferences epr32; 

// --------------------------- variables -----------------------------

struct nvram
{
  String eR_SSID = "";
  String eR_PASS = "";
  int eDEV_CON = 2 ;
  
  unsigned int eEpromInitialised;
   

}eeData;

// -------------------------------- Function declarations -------------------------------------

// ------------------------------------- EEPROM methods --------------------------------------
/***************************************************************************************
* Subroutine : vInitEEPROM
* Tested     :  OK
* Created on :
* Version    :
* Parameters :
* Description: Init EEPROM
***************************************************************************************/
void vInitEEPROM(void)
{
  epr32.begin("EPROMSTORAGE", false);
}
/***************************************************************************************
* Subroutine : vInitDefault_Evalues()
* Tested     :  OK
* Created on :
* Version    :
* Parameters :
* Description: set the eeData to default values if eeprom is corrupted.
* Note       :
***************************************************************************************/

void vInitDefault_Evalues(void)
{
  eeData.eR_SSID="acts";
  eeData.eR_PASS="";
  eeData.eDEV_CON=2;

}

/***************************************************************************************
* Subroutine : vFirstTimeWrite
* Tested     :  OK
* Created on :
* Version    :
* Parameters :
* Description: default values to be written to eprom for first time firmare loading
* Note       :
***************************************************************************************/

void vFirstTimeWrite(void)
{
  vInitDefault_Evalues();
  unsigned int fwriteonce;
  fwriteonce = epr32.getUInt("K_FIRSTTIME", 0); // Reads the first time write flag and updates default values if not written
  if (fwriteonce != 0xaa55)
  { // write only once after firmware uploading

    epr32.putUInt("K_FIRSTTIME", 0xaa55);
    
    epr32.putString("R_SSID",eeData.eR_SSID);
    epr32.putString("R_PASS",eeData.eR_PASS);
    epr32.putInt("DEV_CONN",eeData.eDEV_CON);
    
  }
}

/***************************************************************************************
* Subroutine : vEepromWrite
* Tested     :  OK
* Created on :
* Version    :
* Parameters :
* Description: Write data in eeprom
***************************************************************************************/
void vEepromWrite(void)
{
  Serial.println("Writing data to EEPROM");

  
  Serial.println("SSID: "+ eeData.eR_SSID);
  Serial.println("PASS: "+ eeData.eR_PASS);
  Serial.println("DEV_CONN: "+(String)eeData.eDEV_CON);


  epr32.putString("R_SSID",eeData.eR_SSID);
  epr32.putString("R_PASS",eeData.eR_PASS);
  epr32.putInt("DEV_CONN",eeData.eDEV_CON);
  

}


/***************************************************************************************
* Subroutine : vEepromRead
* Tested     :  OK
* Created on :
* Version    :
* Parameters :
* Description: Read eeprom data
***************************************************************************************/
void vEepromRead(void)
{


  eeData.eEpromInitialised = epr32.getUInt("K_FIRSTTIME");  
  eeData.eR_SSID=epr32.getString("R_SSID");
  eeData.eR_PASS=epr32.getString("R_PASS");
  eeData.eDEV_CON=epr32.getInt("DEV_CONN");
  

  if (eeData.eEpromInitialised != 0xaa55)
  {
    vInitDefault_Evalues(); // init eeData to default values as eeprom is not yet populated with data
  }
  //************************************************

  Serial.println("\n----------------EEPROM DATA------------------");

  Serial.println("SSID: "+ eeData.eR_SSID);
  Serial.println("PASS: "+ eeData.eR_PASS);
  Serial.println("DEV_CONN: "+ (String)eeData.eDEV_CON);

  Serial.println("\n---------------------------------------------");

  // vSet_OP();
}
// --------------------------------- End of EEPROM methods ----------------------------------------



// --------------------------------------- EoF ----------------------------------------------------

#endif
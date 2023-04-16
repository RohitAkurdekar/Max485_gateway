#ifndef ASYNC_TIMER_H
#define ASYNC_TIMER_H

// --------------------------------- variables --------------------------------

volatile unsigned int timer;

//---------------------------------- Function declaration -----------------------------------

// ------------------------------------- Async timers ---------------------------------------

/********************************************************************************
 * @name    : void vASyncTimer()
 * @param   : None
 * @returns : None
 * @date    : 24 Feb 2023
 * @brief   : Async timer which counts in seconds.
 * @note    : Declare timers to be as a global variable with 'unsigned long int'.
              Always call this function in continuous loop. 
 * @test    : tested ok
********************************************************************************/
void vASyncTimer(void)
{
  // vWDT();
 static volatile unsigned long currentTime,oldtime;

 currentTime = millis();
 if (currentTime < oldtime) {oldtime = currentTime;}      // reset old time when millis timer overflows
 if (currentTime >= (oldtime+1000))
 {
  oldtime = currentTime;
    

/*
    if(timer)
    { 
     timer--; 
     if(!timer)
     {
       // Do the needful
     }
    }

*/

 }

}

#endif
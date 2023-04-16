  /********************************************************************************************
 * Name:- MQTT Header
 * Date:- 24/Jan/2023
 * Developer:- Rohit Akurdekar
 * Brief:- MQTT publisher functions.
     * Connect to Network
     * Connect to MQTT broker'
     * Publish message over MQTT
 * Note:- Verify libraries and change Network credentials.
*********************************************************************************************/


#ifndef GATEWAY_H
#define GATEWAY_H

//---------------------------------- Libraries inclusion -----------------------------------
#include <Arduino.h>

#include <WiFi.h>
#include <esp_wifi.h>

#include <PubSubClient.h>           // ESP MQTT library

#include "esp32-hal-gpio.h"

#include "HardwareSerial.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ArduinoJson.h>

#include"customEEPROM.h"
#include"WDT.h"

//---------------------------------- CONSTANTS --------------------------------------------

#define BAUDRATE 19200      // Baudrate

#define BUILTIN_LED 2

#define SKIP_TIME 30              // Skip after 30 seconds

String GATEWAY_ID = "1201";             // UnSecured Series, Max 2 connections, Device 1
#define MAX_CONN 2
// -------------------------- WiFi Credentials --------------------------------------------
// Change according to your MAC Address.
uint8_t MAC_Address[] = {0xCC, 0x50, 0xE3, 0xAB, 0xB5, 0x74};   //CC:50:E3:AB:B5:74                                    

// -------------------------- MQTT Credentials --------------------------------------------

#define MQTT_BROKER  "broker.hivemq.com" // Online broker
// #define MQTT_BROKER  "3.111.57.172"      // EC2 instance
// #define MQTT_BROKER  "192.168.77.205"       // Local desktop

#define TOPIC  "diot/modbus"

#define USERNAME "diotmodbus1"

// #define WIFI_MODE WIFI_MODE_STA
// ---------------------------------------------------------------------------------------
#define LED  2                // LED Pin

#define POWER_LED 4
#define CONNECTIVITY_LED 0
#define UPLOAD_LED 15

#define LED_ON LOW
#define LED_OFF HIGH

// -----------------------------------------------------------------------------

// Set your Static IP address
IPAddress local_IP(192, 168, 77, 123);
// Set your Gateway IP address
IPAddress gateway(192, 168, 77, 255);

IPAddress subnet(255, 255, 255, 0);


// ---------------------------- HTML Page -----------------------------------------

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
<a href=\"/serverIndex\">Return to Home Page</a> <br>
  <form action="/get">
    SSID: <input type="text" name="SSID"><br>
    PASSWORD: <input type="text" name="PASS"><br>
    <input type="submit" value="Submit">
  </form><br>
  ------------------------------------------------------------------------<br>
  <form action="/get">
    Device connected : <input type="number" name="DEV_CONN" min =1 required>
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";

// -------------------- Constructors ------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);

AsyncWebServer server(80);

DynamicJsonDocument JSON(4096);

//---------------------------------- variables --------------------------------------------

long lastMsg = 0;
char msg[50];
int value = 0;
bool isRxdMsg = false;
String RxdMsg;
String message;

int enablePin = 22; 

// int ledpin = 15;


volatile unsigned int timer;

bool fSkip = false;
//---------------------------------- Function declaration -----------------------------------

// ------------------------------------- Async timers ---------------------------------------

/********************************************************************************
 * @name    : void vASyncTimer()
 * @param   : None
 * @returns : None
 * @date    : 24 Feb 2023
 * @brief   : Async timer which counts in seconds.
 * @note    : Declare timers to be as a global variable with 'unsigned long int'.
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
    


    if(timer)
    { 
     timer--; 
     if(!timer)
     {
       fSkip = true;
     }
    }
 }
}

/********************************************************************************
 * @name    : bool isWiFiBegin()
 * @param   : None
 * @returns : WiFi connection status
 * @date    : 26 Jan 2023
 * @brief   : Connect to a WiFi network.
 * @note    : Change MAC address and WIFI credentials according to your network.
 * @test    : tested ok
********************************************************************************/
bool isWiFiBegin()
{

  vEepromRead();  
  esp_wifi_set_mac(WIFI_IF_STA, &MAC_Address[0]);   // Function used to configure MAC address

  Serial.print("MAC address of device: ");  
  Serial.println(WiFi.macAddress());                  // Print MAC address
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(eeData.eR_SSID);

  
  while(eeData.eR_SSID.isEmpty()){
    vWDT();
   }

  WiFi.begin(eeData.eR_SSID.c_str(), eeData.eR_PASS.c_str());   // Password is optional

  while (WiFi.status() != WL_CONNECTED) {           // Verify connection
    delay(500);
    // Serial.print(".");                            // Wait until not connected
  }

  //  if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  // }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());                  // Print IP address

  Serial.println(WiFi.gatewayIP());

  digitalWrite(CONNECTIVITY_LED, LED_ON);              // Turn on connectivity LED

  return true;
}



/********************************************************************************
 * @name    : void vBeginSoftAP()
 * @param   : None
 * @returns : None
 * @date    : 23 Feb 2023
 * @brief   : Configure gateway as an access point.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vBeginSoftAP()
{

  WiFi.softAPdisconnect(true);
  bool fBeginSoftAP = WiFi.softAP("modbus_gateway");//,1,0,4
  if(fBeginSoftAP)
  {
    Serial.println("Soft ap started...");
  }
  else
  {
    Serial.println("Soft ap not started...");
  }
}


/********************************************************************************
 * @name    : void notFound()
 * @param   : None
 * @returns : None
 * @date    : 23 Feb 2023
 * @brief   : Send 'page not found' response to client.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}



/********************************************************************************
 * @name    : void vServer()
 * @param   : None
 * @returns : None
 * @date    : 23 Feb 2023
 * @brief   : Enable a server which hosts a web page to edit WiFi credentials.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vServer()
{
  // Send web page with input fields to client
  server.on("/update_data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam("SSID") && request->hasParam("PASS") ) {

      eeData.eR_SSID = request->getParam("SSID")->value();
      eeData.eR_PASS = request->getParam("PASS")->value();

    }
    if(request->hasParam("DEV_CONN") )
    {
      eeData.eDEV_CON = request->getParam("DEV_CONN")->value().toInt();
    }

    // Serial.println("SSID: "+eeData.eR_SSID);

    // Serial.println("PASS: "+eeData.eR_PASS);

    // Serial.println("Device count: "+(String)eeData.eDEV_CON);
    request->send(200, "text/html", "Data sent to your Gateway.<br><a href=\"/update\">Return to Home Page</a>");

    vEepromWrite();

    vEepromRead();

    delay(30);

    ESP.restart();

  });
  server.onNotFound(notFound);
  server.begin();
}

/********************************************************************************
 * @name    : void vEnableGateway()
 * @param   : None
 * @returns : none
 * @date    : 20 Feb 2023
 * @brief   : Enable MODBUS.
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vEnableGateway()
{

  pinMode(BUILTIN_LED, OUTPUT);

  Serial.begin(BAUDRATE);                   // initialize serial at baudrate 9600:

  pinMode(POWER_LED, OUTPUT);
  pinMode(CONNECTIVITY_LED, OUTPUT);
  pinMode(UPLOAD_LED, OUTPUT);

  digitalWrite(POWER_LED, LED_ON);
  digitalWrite(CONNECTIVITY_LED, LED_OFF);
  digitalWrite(UPLOAD_LED, LED_OFF);

  pinMode(enablePin, OUTPUT);

  delay(10);

  digitalWrite(enablePin, LOW);        //  (Pin 8 always LOW to receive value from Master)
  
}

// --------------------- MQTT ------------------------------------------------

/********************************************************************************
 * @name    : void subscribe()
 * @param   : char* topic, byte* message, unsigned int length
 * @returns : None
 * @date    : 26 Jan 2023
 * @brief   : Subscribe to a MQTT topic
 * @note    : Check TOPIC and logic
 * @test    : tested ok
********************************************************************************/
void subscribe(char* topic, byte* message, unsigned int length) 
{
  String messageTemp;
  
  for (int i = 0; i < length; i++) 
  {
    // Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  // Serial.println();

    RxdMsg = messageTemp;
    isRxdMsg = true;
//   return messageTemp;
}


/********************************************************************************
 * @name    : void vReconnect()
 * @param   : None
 * @returns : None
 * @date    : 26 Jan 2023
 * @brief   : Recconect to MQTT broker 
 * @note    : 
 * @test    : tested ok
********************************************************************************/
void vReconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(USERNAME)) 
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe(TOPIC);
    }
    else 
    {
      // Serial.print("failed, rc=");
      // Serial.print(client.state());
      // Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/********************************************************************************
 * @name    : String sReadData()
 * @param   : None
 * @returns : String containing received data.
 * @date    : 22 Feb 2023
 * @brief   : Read data from all connected devices.
 * @note    : 
 * @test    : 
********************************************************************************/
String sReadData() {
  
  String strArray[eeData.eDEV_CON];
  String rxdData;
  // String rxdData="{\"GATEWAY_ID\":" + (String)GATEWAY_ID;

  JSON["GATEWAY_ID"] = GATEWAY_ID;

  for(int i=1;i<=eeData.eDEV_CON;i++)
  {
    digitalWrite(enablePin, HIGH);
    delay(100);

    Serial.println(("@Device"+(String)i)+"#");


    timer = SKIP_TIME;       // skip after 30 seconds
    fSkip = false;

    Serial.flush();
    delay(25);
    
    digitalWrite(enablePin, LOW);
    delay(15);
    
    String tempData;

    // while(!Serial.available())
    // {  
    //   digitalWrite(BUILTIN_LED, HIGH);
    //   tempData = Serial.readStringUntil('$');
    //   vASyncTimer();  
    //   vWDT();
    //   if(fSkip)
    //   {
        
    //     tempData = "&Data not recieved";
    //     break;
    //   }
    // }

    // delay(20);
    while (Serial.available() >1){
      
      digitalWrite(BUILTIN_LED, LOW);
      tempData = Serial.readStringUntil('$');
      vASyncTimer(); 
      vWDT();
      if(fSkip)
      {
        digitalWrite(BUILTIN_LED, LOW);
        tempData = "&Data not recieved";
        break;
      } 


    }

    if(tempData.isEmpty())
    {
      tempData = Serial.readStringUntil('$');
    }



    // Serial.println("rfd: "+tempData);
    int pos = tempData.indexOf('&');
    tempData = tempData.substring(pos+1);

    tempData.trim();
    if(tempData.length()<5)
    {
      tempData = "Data not recieved";
    }
    // tempData = "\n\ntemp: " + tempData + "\n\n";
    // Serial.println(tempData);
    // rxdData += ",\"Device"+((String)i)+"\":"+tempData;

    JSON["DEVICE"+(String) i] = tempData;

    // rxdData += tempData;
    delay(500);
    // Serial.println("---------------------------\n\n" + (String)rxdData + "\n\n---------------------------");
  }
  // rxdData += "}";

  // Serial.println(rxdData);

  serializeJson(JSON,rxdData);     // JSON stringify | JSON dumps

  return rxdData;
}

/********************************************************************************
 * @name    : void vReadData()
 * @param   : None
 * @returns : None
 * @date    : 22 Feb 2023
 * @brief   : Read data from all connected devices.
 * @note    : 
 * @test    : 
********************************************************************************/
void vSendData() {
  
  // digitalWrite(LED, HIGH);
  long now = millis();
  if (now - lastMsg > 5000) 
  {
    lastMsg = now;
    String data = sReadData();
    

    digitalWrite(enablePin, HIGH);
    delay(100);


    Serial.println(data);

    Serial.flush();

    client.publish(TOPIC, data.c_str());
    digitalWrite(UPLOAD_LED, LED_ON);
    delay(500);
    digitalWrite(UPLOAD_LED, LED_OFF);
    delay(200);

    JSON.clear();
  }
  
}


// ------------------------------- EoF ------------------------------------------

#endif
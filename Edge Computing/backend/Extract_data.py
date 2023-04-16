import insert
import json as JSON
from types import NoneType
import logging
import AWS

def ExtractData(data):
    # print(ExtractDev1Data(data=data))
    # print(ExtractDev2Data(data=data))
    
    # -------- Changes in below lines ---------------------- 
    tempData1 = ExtractDev1Data(data=data)
    if(tempData1 != None):
        insert.insert("Device1",tempData1)
    tempData2= ExtractDev2Data(data=data)
    if(tempData2 != None):  
        insert.insert("Device2",tempData2)



""" ---------------------------------------------------------------------
    @name   : ExtractDev1Data
    @param  : received data 
    @returns: JSON of DEVICE1
    @date   : 01 Mar 2023
    @Tested : OK
    @brief  : Extract data of device 1 from received data
    @note   : 
--------------------------------------------------------------------- """
def ExtractDev1Data(data):
    DEVICE1  = None               # Change data to None
    try:
        if(data["DEVICE1"] == 'Data not recieved'):
             logging.log(1,"Data not recieved from Device1")              # uncomment this line
        elif(isinstance(data["DEVICE1"],str)):
            DEVICE1 = JSON.loads(data["DEVICE1"])
            # print(DEVICE1)
        elif(isinstance(data["DEVICE1"],dict)):
            DEVICE1 = data["DEVICE1"]
            # print(DEVICE1)
        else:
            DEVICE1 = None
    except Exception as e:
        print(e)

    if(int(DEVICE1["TEMPERATURE"]) > 33):
        AWS.Device1Cloud(DEVICE1["TEMPERATURE"])
    return DEVICE1
""" ---------------------------------------------------------------------
    @name   : ExtractDev2Data
    @param  : received data 
    @returns: JSON of DEVICE2
    @date   : 01 Mar 2023
    @Tested : OK
    @brief  : Extract data of device 2 from received data
    @note   : 
--------------------------------------------------------------------- """
def ExtractDev2Data(data):
    DEVICE2 = None            # Change data to None
    try:
        if(data["DEVICE2"] == 'Data not recieved'):
           logging.log(1,"Data not recieved from Device2")       # uncomment this line
                                                            
        elif(isinstance(data["DEVICE2"],str)):
            DEVICE2 = JSON.loads(data["DEVICE2"])
            # print(DEVICE2)
        elif(isinstance(data["DEVICE2"],dict)):
            DEVICE2 = data["DEVICE2"]
            # print(DEVICE2)
        else:
            DEVICE2 = None
    except Exception as e:
        print(e)
    if(int(DEVICE2["VALUE"]) > 3300):
        AWS.Device1Cloud(DEVICE2["VALUE"])
    return DEVICE2


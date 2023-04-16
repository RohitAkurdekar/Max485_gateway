#!/usr/bin/python3

# import paho.mqtt.client as mqtt
import paho.mqtt.publish as publisher
import json as JSON

# CONSTANTS
TOPIC       = 'diot/modbus'
#BROKER_ADDR = "broker.hivemq.com"
BROKER_ADDR = "192.168.77.65"
PORT        = 1883
KEEP_ALIVE  = 60

# Callback method for successful connection
def on_connect(client, userData, flags, responseCode):
    print("Connected to MQTT broker")
    publisher.subscribe(TOPIC)


# Defining callback methods in MQTT  Client
publisher.on_connect = on_connect


data = {
  "GATEWAY_ID":1301,
  "DEVICE1": 
  { "DEVICE_ID":"BMP280_1",
    "temperature":31.24,
    "pressure":94150.06,
    "altitude":615.26
  },
  "DEVICE2":
  {
    "DEVICE_ID":"POT_1",
    "value":25.00
  }
}


# data = "{\"GATEWAY_ID\":1301,\"Device1\": {\"DEVICE_ID\":BMP280_1,\"temperature\":31.24,\"pressure\":94150.06,\"altitude\":615.26},\"Device2\":{\"DEVICE_ID\":POT_1,\"value\":441.00}}"


data = JSON.dumps(data)

while True:
    
    print(data)
    # Send single publish request to broker
    publisher.single(topic=TOPIC,
                    payload=data,
                    hostname=BROKER_ADDR,
                    port=PORT,
                    qos=2,
                    keepalive=KEEP_ALIVE)
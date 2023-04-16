#!/usr/bin/python3

import paho.mqtt.client as mqtt
import logging
import Extract_data as ED

import json as JSON

def mqtt_sub():

    # To subscribe
    subscriber = mqtt.Client()

    # CONSTANTS
    TOPIC       = 'diot/modbus'
    #BROKER_ADDR = "192.168.77.205"
    BROKER_ADDR = "broker.hivemq.com"
    PORT        = 1883
    KEEP_ALIVE  = 60

    # Callback method for successful connection
    def on_connect(client, userData, flags, responseCode):
        logging.log(1,"Connected to MQTT broker")
        subscriber.subscribe(TOPIC)
        return "Subscribed"

    # Callback method to print recieved message
    def on_message(client,userData, msg):
        # print("\n-------------------------------------------------------\n")
        # print('Topic: ' + msg.topic + ' Message: ' + str(msg.payload))
        # print("\n-------------------------------------------------------\n")
        try:
            data = JSON.loads(msg.payload.decode('utf-8'))
        except Exception as e:
            print(e)
        # print(data['GATEWAY_ID'])
        logging.log(1,data)
        ED.ExtractData(data=data)
        return data

    # Defining callback methods in MQTT  Client
    subscriber.on_connect = on_connect
    subscriber.on_message = on_message

    # Send connection request to broker
    subscriber.connect(host=BROKER_ADDR,port=PORT,keepalive=KEEP_ALIVE)

    # To maintain the connection
    subscriber.loop_forever()

    
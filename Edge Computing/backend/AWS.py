# Program to perform simple GET request to HTTP server
import requests

HOST = " https://0w7omsm2z1.execute-api.ap-south-1.amazonaws.com"
PATH1 = "/getValueStage/device1?"
PATH2 = "/getValueStage/device2?"


"""
    Send data to cloud using api request.
"""
def Device1Cloud(data):
    DATA = "pressure=" + str(data)

    # Make HTTP GET Request to Server
    response = requests.get(HOST + PATH1 + DATA)


    # Printing the JSON response with inbuilt encoder
    print(response.json())
    # print(response.status_code)


"""
    Send data to cloud using api request.
"""
def Device2Cloud(data):
    DATA = "value=" + str(data)

    # Make HTTP GET Request to Server
    response = requests.get(HOST + PATH2 + DATA)


    # Printing the JSON response with inbuilt encoder
    print(response.json())
    # print(response.status_code)

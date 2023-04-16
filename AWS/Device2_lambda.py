# Device 1 AWS LAMBDA method
import json
import boto3
import time
import calendar

sns = boto3.client('sns')

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('<Your table name>')


# Update this
emailARN = "<Your ARN for email>"
smsARN = "<Your ARN for SMS>"

mydata = {}


def lambda_handler(event, context):
    
    # value is keyword used in API gateway. 
    value = event['queryStringParameters']['value']

    mydata['value'] = value
    try:
        # store data in DynamoDB
        insert_data(value)
        # implement your business logic here
        if(int(value)>3300):                
            send_SMS()
            send_EMAIL()
        
    except Exception as e:
        mydata['error'] = e

    responseObject = {}
    
    responseObject['statusCode'] = 200
    responseObject['headers'] = {}
    responseObject['headers']['Content-Type'] = 'application/json'
    responseObject['body'] = json.dumps(mydata)
    
    return responseObject
    
'''
* Name  : insert_data(temperature)
* Params: value
* Brief : Store data in DynamoDB
'''
def insert_data(value):
    
        current_GMT = time.gmtime()
        
        time_stamp = calendar.timegm(current_GMT)
        mydata['timestamp'] = time_stamp 
        
        # update column names according to DynamoDB
        responseDynamo = table.put_item(Item={"id": time_stamp,"value":value})
              
'''
* Name  : send_SMS()
* Params: None
* Brief : Send SMS to user
'''
def send_SMS():
    
            mydata['Warning'] = "Readings at critical level. value = {0}".format(mydata['value'])
            responseSNS = sns.publish(
                    TopicArn = smsARN,
                    Message=str(mydata['value'])
                )
                 
'''
* Name  : send_EMAIL()
* Params: None
* Brief : Send Email to user
'''      
def send_EMAIL():
            responseSNS = sns.publish(
                    TopicArn = emailARN,
                    Message="Device 2 value has reached critical level. Value = " + str(mydata['value'])
                )
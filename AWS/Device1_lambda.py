# Device 1 AWS LAMBDA method
# Please update ARN according your system generated ARN
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
    
    # temperature is keyword used in API gateway. 
    temperature = event['queryStringParameters']['temperature']

    mydata['temperature'] = temperature
    try:
        # store data in DynamoDB
        insert_data(temperature)
        # implement your business logic here
        if(int(temperature)>3300):                
            send_SMS()
            send_EMAIL()
        
    except Exception as e:
        mydata['error'] = e.args

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
        responseDynamo = table.put_item(Item={"ID": time_stamp,"value":value})
        
'''
* Name  : send_SMS()
* Params: None
* Brief : Send SMS to user
'''
def send_SMS():
    
            mydata['Warning'] = "Temperature Readings at critical level. value= {0}".format(mydata['temperature'])
            responseSNS = sns.publish(
                    TopicArn = smsARN,
                    Message= str(mydata['Warning'])
                )
               
'''
* Name  : send_EMAIL()
* Params: None
* Brief : Send Email to user
''' 
def send_EMAIL():
            responseSNS = sns.publish(
                    TopicArn = emailARN,
                    Message="Device 1 temperature has reached critical level. temperature = {0}".format(mydata['temperature'])
                )
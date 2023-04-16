
import datetime
from zoneinfo import ZoneInfo
import pymongo
from pymongo.errors import DuplicateKeyError
import logging
import sys
from bson.timestamp import Timestamp

url = "modbus_mongodb"
db_name = "Modbus"

def insert(COLLECTION_NAME,DATA):
    collection_name = COLLECTION_NAME

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
        

 
    date_time= datetime.datetime.now() 
    time_stamp=date_time.strftime("%m/%d/%Y, %H:%M:%S")
    #print(time_stamp)
    DATA["_id"]=time_stamp

    document = DATA

    
    try:
         collection.insert_one(document)

    except DuplicateKeyError as e:
        #logging.error("Insert failed. Document already exists in the collection.")
        return False

    except IOError as e:
        logging.error("I/O error({0}): {1}".format(e.errno, e.strerror))
        return False

    except Exception as e:
        logging.error("Unexpected error:", sys.exc_info()[0])
        return False
    

    # if all is good
    return True

    



"""


from pymongo import MongoClient

def insert(document):
    # create a client object to connect to MongoDB
    client = MongoClient('mongodb://192.168.77.205:27017/')

    # specify the database and collection
    db = client['mydatabase']
    collection = db['mycollection']

    # insert the document into the collection
    result = collection.insert_one(document)

    # print the inserted document's _id
    print(result.inserted_id)

# create a document to insert
document = {
    'name': 'John Doe',
    'age': 30,
    'email': 'johndoe@example.com'
}

"""
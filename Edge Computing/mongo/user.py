import pymongo
from pymongo.errors import DuplicateKeyError
import logging
import sys
import hashlib

url = "modbus_mongodb"
db_name = "Modbus"


def insert_user(username,password,Email,Contact_No):
    collection_name = "User_Details"

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
        
    user = {
            "Username": username, "password":hashlib.sha256(password.encode()).hexdigest(),
            "Email":Email,"Contact":Contact_No
            }
    if collection.find_one({'Email': Email}):
        return "Username already exists"
   
    try:
         collection.insert_one(user)

    except DuplicateKeyError as e:
        return False

    except IOError as e:
        logging.error("I/O error({0}): {1}".format(e.errno, e.strerror))
        return False

    except pymongo.errors.ServerSelectionTimeoutError:
        logging.error("Unexpected error:", sys.exc_info()[0])
        return False
   
    return True

   
def get_user(Email,password):
    collection_name = "User_Details"

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
        
    hashed_password = hashlib.sha256(password.encode()).hexdigest()

    results = collection.find_one({'Email': Email, 'password': hashed_password})

    if results:
        return True
    else:
        return False
    
def details(Email,password):
    collection_name1 = "User_Details"
    collection_name2 = "Login_Details"

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection1 = db[collection_name1]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection1 = db[collection_name1]
        
    hashed_password = hashlib.sha256(password.encode()).hexdigest()

    results = collection1.find_one({'Email': Email, 'password': hashed_password})
    

    collection2 = db[collection_name2]

    try:
         collection2.insert_one(results)

    except DuplicateKeyError as e:
        return False

    except IOError as e:
        logging.error("I/O error({0}): {1}".format(e.errno, e.strerror))
        return False

    except pymongo.errors.ServerSelectionTimeoutError:
        logging.error("Unexpected error:", sys.exc_info()[0])
        return False
   
    return True

    
def logged_user():
    collection_name = "Login_Details"

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
        
    results = collection.find({})

    return results



def logged_delete():
    collection_name = "Login_Details"

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
        

    results = collection.find({})

    collection.delete_one({"_id":results[0]["_id"]})

    







    #     def get(COLLECTON_NAME,ID):
    # collection_name = COLLECTON_NAME

    # client = pymongo.MongoClient(url)
    # db = client[db_name]
    # collection = db[collection_name]

    # query = {"_id": ID}
    # projection = {"field2": 1, "field3": 1}

    # results = collection.find(query, projection)

    # for result in results:
    #     print(result)
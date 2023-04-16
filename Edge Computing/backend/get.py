import pymongo

url = "modbus_mongodb"
db_name = "Modbus"

def get_all(COLLECTON_NAME):
    collection_name = COLLECTON_NAME

    try:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
    except pymongo.errors.ServerSelectionTimeoutError:
        client = pymongo.MongoClient(url)
        db = client[db_name]
        collection = db[collection_name]
        

    # using find function
    # cursor = collection.find({})

    # using find_all function
    data = list(collection.find({}))

    # using cursor
    
    return data

    # using list
    # for document in data:
    #     print(document)


def get_last_30(COLLECTON_NAME):
    cursor = get_all(COLLECTON_NAME)

    cursor.reverse()

    index = 0

    last_30_result = [None] * 30

    for document in cursor:
        #print(document)
        last_30_result[index] = document
        index +=1
        if(index == 30):
            break

    last_30_result.reverse()

    return last_30_result


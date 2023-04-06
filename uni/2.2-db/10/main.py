import datetime

from pymongo import MongoClient

# client = MongoClient("mongodb://hostname:port")
client = MongoClient("mongodb://localhost")  # will connect to localhost and default port 27017

db = client['test']

# cursor = db.collection_name.find({key1: value, key2: value2 ...})
# cursor = db..find()

# === TASK 1 ===
irish_list = list(db.restaurants.find({'cuisine': 'Irish'}))
irish_russian_list = list(db.restaurants.find({'$or': [
    {'cuisine': 'Irish'},
    {'cuisine': 'Russian'},
]}))

# Prospect Park West 284, 11215
db.restaurants.find({
    'address.street': 'Prospect Park West',
    'address.building': '284',
    'address.zipcode': '11215'
})


# === TASK 2 ===

# result = db.collection_name.insert_one({key1: value, key2: value2 ...})
# result = db.collection_name.insert_many([{key1: value, key2: value2 ...}, {...}, ...])

def my_insert():
    new_doc = {
        'address': {
            'building': '126',
            'coord': [-73.9557413, 40.7720266],
            'street': 'Sportivnaya',
            'zipcode': '420500'
        },
        'borough': 'Innopolis',
        'cuisine': 'Serbian',
        'grades': [{
            'date': datetime.datetime(2023, 4, 4),
            'grade': 'A',
            'score': 11
        }],
        'name': 'The Best Restaurant',
        'restaurant_id': '41712354'
    }

    result = db.collection_name.insert_one(new_doc)
    assert result.acknowledged


my_insert()

# === TASK 3 ===

result = db.restaurants.delete_one({'borough': 'Brooklyn'})
assert result.acknowledged

result = db.restaurants.delete_many({'cuisine': 'Thai'})
assert result.acknowledged


# === TASK 4 ===

def func(addr_street: str):
    res_docs = list(db.restaurants.find({'address.street': addr_street}))

    for i in res_docs:
        cnt_a = len(list(filter(lambda x: x['grade'] == 'A', i['grades'])))
        if cnt_a >= 1:
            i_id = i['_id']
            db.restaurants.delete_one({'_id': i_id})
        else:
            pass


func('Prospect Park West')


#!/usr/bin/env python
# coding: utf-8

# In[10]:


from pymongo import MongoClient
from bson.objectid import ObjectId

class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self):
        # Initialize MongoDB connection
        USER = 'aacuser'
        PASS = 'ThisIsMyPassword'
        HOST = 'nv-desktop-services.apporto.com'
        PORT = 33324
        DB = 'AAC'
        COL = 'animals'

        #Connect to the MongoDB server and access database
        self.client = MongoClient('mongodb://%s:%s@%s:%d' % (USER,PASS,HOST,PORT))
        self.database = self.client['%s' % (DB)]
        self.collection = self.database['%s' % (COL)]

    #Create
    def create(self, data):
        """Insert a document into the MongoDB collection"""
        if data is not None:
            #Check for existing animal id
            existing = self.database.animals.find_one({"animal_id": data["animal_id"]})
            if existing:
                print("animal_id already exists")
                return False
            else:
                #Create new animal document
                self.database.animals.insert_one(data)
                return True
        else:
            raise Exception("Nothing to save, because data parameter is empty")

    #Read
    def read(self, query):
        """Query documents from the MongoDB collection"""
        if query is not None:
            results = self.database.animals.find(query)
            #Search for animal document and print as a list
            return list(results)
        else:
            raise Exception("Nothing to save, because data parameter is empty")

    #Update
    def update(self, query, new_data):
        if query is not None and new_data is not None:
            #Update animal document with new data for sets provided
            result = self.database.animals.update_one(query, {"$set": new_data})
            #Print 1 or 0 for documents updated 
            print("Objects Modified:")
            return result.modified_count
        else:
            raise Exception("Both query and new_data parameters must be provided")
    
    #Delete
    def delete(self, query):
        if query is not None:
            #Delete animal document based on animal id
            result = self.database.animals.delete_one(query)
            #Print 1 or 0 for documents deleted
            print("Objects Deleted:")
            return result.deleted_count
        else:
            raise Exception("Query parameter must be provided")


# In[ ]:





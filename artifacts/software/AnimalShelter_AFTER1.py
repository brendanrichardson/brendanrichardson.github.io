#!/usr/bin/env python
# coding: utf-8

# In[10]:

import os 
from typing import Any, Dict, List, Optional, Tuple

from pymongo import MongoClient, ASCENDING
from pymongo.errors import DuplicateKeyError, PyMongoError

class MongoDBClient:
    # Handles MongoDB connection and collection access
    # Removed hardcoded credentials for enhancement
    # Index creation for performance and integrity

    def __init__(self):
        # Initialize MongoDB connection

        self.uri = os.getenv("MONGO_URI", "").strip()
        self.db_name = os.getenv("MONGO_DB", "AAC").strip()
        self.collection_name = os.getenv("MONGO_COLLECTION", "animals").strip()

# THIS IS ONLY INCLUDED FOR BACKWARD COMPATIBLE FALLBACK IN SNHU LAB
#        if not self.uri:
#            USER = os.getenv("MONGO_USER", "aacuser")
#            PASS = os.getenv("MONGO_PASS", "ThisIsMyPassword")
#            HOST = os.getenv("MONGO_HOST", "nv-desktop-services.apporto.com")
#            PORT = int(os.getenv("MONGO_PORT", "33324"))
#            self.uri = f"mongodb://{USER}:{PASS}@{HOST}:{PORT}"

        self.client = MongoClient(self.uri)
        self.database = self.client[self.db_name]
        self.collection = self.database[self.collection_name]

        self._ensure_indexes()
        
    def _ensure_indexes(self) -> None:
        """ Create indexes to improve performance and enforce data integrity """

        # Enforce unique animal_id at the database level
        self.collection.create_index([("animal_id", ASCENDING)], unique=True)

        # Helpful indexes for common filters/search
        self.collection.create_index([("animal_type", ASCENDING)])
        self.collection.create_index([("breed", ASCENDING)])
        self.collection.create_index([("name", ASCENDING)])

class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self):
        # Initialize MongoDBClient connection
        self.db = MongoDBClient()

    #Create
    def create(self, data: Dict[str, Any]) -> bool:
        """Insert a document into the MongoDB collection"""
        if not data:
            raise ValueError("Nothing to save, because data parameter is empty")

        if "animal_id" not in data or not data ["animal_id"]:
            raise ValueError("animal_id is required")
        
        try:
            self.db.collection.insert_one(data)
            return True
        except DuplicateKeyError: 
            # Unique index ensures this is safe even under concurrent inserts
            return False
        except PyMongoError as e:
            raise RuntimeError(f"Database insert failed: {e}")


    #Read
    def read(self,
            query: Dict[str, Any],
            projection: Optional[Dict[str, int]] = None,
            sort: Optional[List[Tuple[str, int]]] = None,
            limit: int = 0,
            skip: int = 0) -> List[Dict[str, Any]]:
        
        """Query documents from the MongoDB collection"""
        """ Return only needed fields for faster, smaller payloads """
        """ Sort/limit/skip supports pagination and efficient UI """

        if query is None:
            raise ValueError("Nothing to read, because query parameter is empty")
        
        try:
            cursor = self.db.collection.find(query, projection)

            if sort:
                cursor = cursor.sort(sort)
            if skip > 0: 
                cursor = cursor.skip(skip)
            if limit > 0:
                cursor = cursor.limit(limit)

            return list(cursor)
        except PyMongoError as e:
            raise RuntimeError(f"Database read failed: {e}")
        

    #Update
    def update(self, query: Dict[str, Any], new_data: Dict[str, Any]) -> int:
        if not query or not new_data:
            raise ValueError("Both query and new_data parameters must be provided")
        
        try:
            result = self.db.collection.update_one(query, {"$set": new_data})
            return int(result.modified_count)
        except PyMongoError as e:
            raise RuntimeError(f"Database update failed: {e}")
    
    #Delete
    def delete(self, query: Dict[str, Any]) -> int:
        if not query:
            raise ValueError("Query parameter must be provided")
        
        try:
            result = self.db.collection.delete_one(query)
            return int(result.deleted_count)
        except PyMongoError as e:
            raise RuntimeError(f"Database delete failed: {e}")
        
    #Aggregation (enhancement)
    def count_by_field(self, field_name: str, match: Optional[Dict[str, Any]] = None) -> List[Dict[str, Any]]:

        """ Returns counts grouped by a given field using MongoDB aggregation """
        if not field_name:
            raise ValueError("field_name must be provided")

        pipeline = []
        if match:
            pipeline.append({"$match": match})

        pipeline.extend([
            {"$group": {"_id": f"${field_name}", "count": {"$sum": 1}}},
            {"$sort": {"count": -1}}
        ])

        try:
            return list(self.db.collection.aggregate(pipeline))
        except PyMongoError as e:
            raise RuntimeError(f"Aggregation failed: {e}")
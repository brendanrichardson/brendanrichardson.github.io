[Home](index.md)

# Enhancement 3: Databases

**Artifact:** AnimalShelter.py + Dashboard Notebooks (CS 340)  
**Category:** Databases

## Before and After
- **Original artifact:** *[artifacts/databases/AnimalShelter_BEFORE.py](https://github.com/brendanrichardson/brendanrichardson.github.io/blob/main/artifacts/databases/AnimalShelter_BEFORE.py)*
- **Enhanced artifact:** *[artifacts/databases/AnimalShelter_AFTER3.py](https://github.com/brendanrichardson/brendanrichardson.github.io/blob/main/artifacts/databases/AnimalShelter_AFTER3.py)*

## Narrative
*[artifacts/narratives/CS-499 Milestone Four.pdf](https://github.com/brendanrichardson/brendanrichardson.github.io/blob/main/artifacts/narratives/CS-499%20Milestone%20Four.pdf)*

The artifact I enhanced for Milestone Four is AnimalShelter.py, the backend module from my CS 340 project that connects to a MongoDB database and performs CRUD operations on an animals collection. I originally created this artifact during CS 340 as part of an analytics focused dashboard application that required reliable database access for reading and managing animal records. The file serves as the data access layer for the project, meaning it is responsible for connecting to the database, executing queries and returning results that can be used by a dashboard or other client code.

I selected this artifact for my ePortfolio because it demonstrates practical database skills that apply directly to real world data work, especially for analytics use cases. This module shows how to connect to a database, enforce data rules, retrieve records efficiently and support analysis ready outputs. Since my career goal is data analytics, I wanted a database focused artifact that clearly shows I understand how data is stored, validated and retrieved in a way that supports accurate reporting and decision making.

To improve the artifact, I focused on enhancements that are specifically database centered and measurable. First, I removed hardcoded connection credentials and shifted configuration to environment variables such as MONGO_URI, MONGO_DB and MONGO_COLLECTION. This reduces the risk of exposing information when hosting the project on GitHub and demonstrates a stronger security mindset related to database access. Second, I implemented database indexes to improve performance and enforce integrity rules at the database level. Most importantly, I created a unique index on animal_id to prevent duplicate records even under concurrent inserts, which is more reliable than application only checks. I also added additional indexes on commonly filtered fields such as animal type, breed, and name to support faster queries as the dataset grows.

Beyond integrity and performance, I enhanced the Read operation to support database-efficient querying. The Read method now accepts optional projection, sorting and pagination parameters. This makes data retrieval more scalable by returning only the fields needed for the dashboard and allowing results to be paged rather than retrieving an entire collection at once. Finally, I added an Aggregation method that uses MongoDB’s aggregation pipeline to generate grouped counts. This enhancement directly supports analytics and reporting by shifting summary computations into the database, which is both efficient and aligned with real world analytics workflows.

This milestone supports the course outcomes related to implementing computing solutions that deliver value using appropriate tools and techniques, as well as developing a security mindset. Using environment variables for the MongoDB connection improves privacy and reduces the risk of credential exposure. Creating indexes and enforcing uniqueness at the database level demonstrates the ability to build a more professional and scalable database solution. The enhanced Read method and the new Aggregation function demonstrate database query optimization and analytics readiness, which aligns with the outcome involving industry specific goals and data driven decision support. My outcome-coverage plan from Module One still applies, and this enhancement strengthens my database coverage by adding integrity enforcement, performance optimization and analytical querying.

Working on this enhancement reinforced that database quality is not just about being able to connect and run CRUD operations. Reliable database systems depend on thoughtful design decisions such as enforcing uniqueness constraints, using indexes to support common query patterns, and retrieving data efficiently. I also learned that it’s important to build database functions in a way that supports downstream analysis, such as returning only necessary fields and providing summary outputs through aggregation rather than pushing everything to the client.

The most challenging part of the process was making changes that were clearly database enhancements rather than general refactoring. I wanted the improvements to be visible and defensible in terms of integrity, performance and analytics value. Ensuring that the new database constraints and query features still worked cleanly also required careful attention, because moving to environment-based configuration changes how the code is executed. Overall, the enhancement process improved my confidence in designing data access code that is safer, more scalable and better suited to analytics driven applications.

[Home](index.md)

# Enhancement 1: Software Design & Engineering

**Artifact:** AnimalShelter.py (CS 340)  
**Category:** Software Design and Engineering

## Before and After
- **Original artifact:** *artifacts/software/AnimalShelter_BEFORE.py*
- **Enhanced artifact:** *artifacts/software/AnimalShelter_AFTER1.py*

## Narrative
*artifacts/narratives/CS-499 Milestone Two.docx*

  The artifact I selected for this enhancement is the AnimalShelter.py backend module from my CS-340 course. This file was originally created as part of a project designed to support an analytics dashboard that interacts with a MongoDB database containing animal shelter records. The module provides core CRUD functionality and serves as the interface between the database and multiple Jupyter Notebook files used for querying and visualization. Because this artifact plays a central role in managing data access and application logic, it provides a strong foundation for demonstrating software design and engineering principles.
	I selected this artifact for my ePortfolio because it demonstrates backend software development skills that are directly applicable to data focused applications. The original implementation was functional but tightly coupled with database configuration, connection logic, and CRUD operations all handled within a single class. While this approach worked for the initial project, it limited maintainability, scalability, and clarity as the application could grow more complex. 
	To improve the design, I enhanced the artifact by separating database connection responsibilities into a dedicated MongoDBClient class while preserving the original CRUD functionality within the AnimalShelter class. This change introduced a clearer separation of concerns, allowing each class to focus on a specific responsibility. The refactor improved readability, reduced coupling, and made the codebase easier to reason about and extend in the future. Importantly, the enhancement was scoped to avoid breaking existing Jupyter Notebook workflows, ensuring that analytical functionality remained intact.
	This enhancement aligns with the course outcomes I identified in Module One, particularly those related to software design and engineering. By refactoring the code to improve structure and maintainability, I demonstrated the ability to design and evaluate computing solutions using established software engineering principles while managing trade-offs between improvement and stability. The enhancement also reflects the use of well founded techniques and tools to deliver a computing solution that supports data driven decision making. The outcome coverage remains consistent with my original plan. However, implementing the enhancement reinforced how early architectural decisions can affect long term maintainability and highlighted areas, such as secure credential management, that could be addressed in future iterations.
	Enhancing this artifact helped me better understand the importance of intentional software design, especially in applications that support data analysis. One key lesson was recognizing how combining multiple responsibilities into a single class can make code harder to maintain and reason about, even when the application appears to work correctly. By refactoring the design and introducing a separate database client, I gained experience applying separation of concerns in a practical context.
	A challenge during this process was balancing improvement with scope. It would have been easy to expand the enhancement into a full project restructure, but limiting the changes to a single file refactor ensured stability and alignment with the milestone requirements. This experience reinforced the importance of making incremental, justified improvements rather than overengineering solutions. Overall, the enhancement strengthened my understanding of professional software design practices and their role in building reliable, data driven systems.


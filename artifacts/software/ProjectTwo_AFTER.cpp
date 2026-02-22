//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Brendan Richardson
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Enhanced "Course Planning Program to Store Course Information" for CS499
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

//Define the Course struct to hold course data
struct Course {
    string courseNumber;           //The unique identifier for the course
    string courseTitle;            //The title of the course
    unordered_set<string> prerequisites;  //A list of prerequisite courses
};

//Function to convert a string to uppercase
string toUpper(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

//First Enhancement

//Build adjaccency list
unordered_map<string, vector<string>>buildPrereqGraph(const unordered_map<string, Course>& courseTable) {
    unordered_map<string, vector<string>> graph;
    graph.reserve(courseTable.size());

    for (const auto& pair : courseTable) {
        const Course& course = pair.second;
        vector<string> prereqs;
        prereqs.reserve(course.prerequisites.size());

        for (const auto& p : course.prerequisites) {
            prereqs.push_back(toUpper(p));
        }

        graph[toUpper(course.courseNumber)] = prereqs;
    }

    return graph;
}

//DFS helper for cycle detection
bool dfsDetectCycle(
    const string& node,
    const unordered_map<string, vector<string>>& graph,
    unordered_map<string, int>& state,
    vector<string>& pathStack,
    vector<string>& cycleOut
) {
    state[node] = 1;
    pathStack.push_back(node);

    auto it = graph.find(node);
    if (it != graph.end()) {
        for (const string& neighbor : it->second) {

            //Only traverse prereqs that exist as nodes in the graph
            if (graph.find(neighbor) == graph.end()) {
                continue;
            }

            if (state[neighbor] == 0) {
                if (dfsDetectCycle(neighbor, graph, state, pathStack, cycleOut)) {
                    return true;
                }
            }
            else if (state[neighbor] == 1) {
                //Extract the cycle path from where neighbor appears in pathStack
                auto pos = find(pathStack.begin(), pathStack.end(), neighbor);
                if (pos != pathStack.end()) {
                    cycleOut.assign(pos, pathStack.end());
                    cycleOut.push_back(neighbor);
                }

                return true;
            }
        }
    }

    state[node] = 2;
    pathStack.pop_back();
    return false;
}

//Returns true if cycle exists, outputs an example cycle path
bool hasPrerequisiteCycle(const unordered_map<string, Course>& courseTable, vector<string>& cyclePath) {
    auto graph = buildPrereqGraph(courseTable);

    unordered_map<string, int> state;
    state.reserve(graph.size());

    for (const auto& pair : graph) {
        state[pair.first] = 0; //initiialize
    }

    vector<string> stack;

    for (const auto& pair : graph) {
        const string& node = pair.first;
        if (state[node] == 0) {
            if (dfsDetectCycle(node, graph, state, stack, cyclePath)) {
                return true;
            }
        }
    }

    return false;
}

//Second Enhancement

// Prefix search using sorted index and binary search

vector<string> buildSortedCourseIndex(const unordered_map<string, Course>& courseTable) {
    vector<string> ids;
    ids.reserve(courseTable.size());

    for (const auto& pair : courseTable) {
        ids.push_back(pair.first); 
    }

    sort(ids.begin(), ids.end());
    return ids;
}

//Returns matching course IDs using lower_bound and scan until prefix mismatch
vector<string> searchByPrefix(const vector<string>& sortedIds, const string& rawPrefix) {
    vector<string> results;
    string prefix = toUpper(rawPrefix);

    if (prefix.empty())
        return results;

    auto startIt = lower_bound(sortedIds.begin(), sortedIds.end(), prefix);

    for (auto it = startIt; it != sortedIds.end(); ++it) {
        //if current string no longer starts with prefix, stop
        if (it->rfind(prefix, 0) != 0) {
            break;
        }

        results.push_back(*it);
    }

    return results;
}

//Function to read course data from a file
vector<Course> readCourseData(const string& fileName) {
    ifstream file(fileName);        //Attempt to open the specified file
    vector<Course> courses;         //Vector to store the courses
    unordered_map<string, bool> courseLookup; //Map to store course numbers for validation

    //Check if the file was opened successfully
    if (!file.is_open()) {
        cout << "File '" << fileName << "' cannot be opened." << endl; //Error message if file cannot be opened
        return courses;                           //Return empty course vector
    }

    string line;

    //Read each line from the file
    while (getline(file, line)) {
        if (line.empty()) continue;  //Skip empty lines

        stringstream ss(line);   //String stream to process the line
        string courseNumber, courseTitle, prerequisite;
        unordered_set<string> prerequisites;

        //Extract course number and title
        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');

        //Added courseNumber to uppercase for consistency
        courseNumber = toUpper(courseNumber);

        //Extract prerequisites if any
        while (getline(ss, prerequisite, ',')) {
            prerequisite = toUpper(prerequisite);
            if (!prerequisite.empty()) {
                prerequisites.insert(prerequisite);
            }
        }

        //Create a Course object and add it to the courses vector
        Course course = { courseNumber, courseTitle, prerequisites };
        courses.push_back(course);
        courseLookup[courseNumber] = true;  //Add course number to the lookup map
    }

    //Validate the prerequisites to ensure they exist in the course list
    for (const auto& course : courses) {
        for (const auto& prerequisite : course.prerequisites) {
            if (courseLookup.find(prerequisite) == courseLookup.end()) {
                //Error if a prerequisite is not found in the course lookup
                cout << "Prerequisite course " << prerequisite << " does not exist." << endl;
                return {};  //Return empty course vector on error
            }
        }
    }

    return courses;  // Return the list of courses
}

//Function to print course information and prerequisites
void printCourseInformation(const unordered_map<string, Course>& courseTable, const string& targetCourseNumber) {
    //Convert the input to uppercase to handle case-insensitive search
    string upperCourseNumber = toUpper(targetCourseNumber);

    //Check if the course exists in the hash table
    auto it = courseTable.find(upperCourseNumber);
    if (it != courseTable.end()) {
        const Course& course = it->second;

        //Print course number and title
        cout << "\n" << course.courseNumber << " - " << course.courseTitle << endl;
        //Print prerequisites or indicate that there are none
        if (course.prerequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        }
        else {
            vector<string> prereqList(course.prerequisites.begin(), course.prerequisites.end());
            sort(prereqList.begin(), prereqList.end());
            
            cout << "Prerequisites: ";
            for (size_t i = 0; i < prereqList.size(); ++i) {
                cout << prereqList[i];
                if (i + 1 < prereqList.size())
                    cout << ", ";
            }
            cout << endl;
        }
    }
    else {
        cout << "Course not found." << endl;  //Error message if course is not found
    }
}

//Function to load courses into a hash table
unordered_map<string, Course> loadCoursesIntoCourseTable(const vector<Course>& coursesList) {
    unordered_map<string, Course> courseTable;  //Hash table to store courses
    courseTable.reserve(coursesList.size());

    for (const auto& course : coursesList) {
        courseTable[course.courseNumber] = course;  //Insert each course into the hash table. 
        //Took away toUpper because that was established earlier in readCourseData
    }
    return courseTable;  //Return the populated hash table
}

//Function to sort and print courses in alphanumeric order
void sortAndPrintCourses(const unordered_map<string, Course>& courseTable) {
    //Check if the hash table is empty
    //Changed logic to clean up code
    if (courseTable.empty()) {
        cout << "Course table is empty. Please load data first." << endl;
        return;
    }

    vector<Course> sortedCourses;
    sortedCourses.reserve(courseTable.size());

    for (const auto& pair : courseTable) {
        sortedCourses.push_back(pair.second);
    }

    sort(sortedCourses.begin(), sortedCourses.end(),
        [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
        });

    for (const auto& course : sortedCourses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

//Function to display the menu and handle user choices
void printHeader() {
    cout << "ABCU COURSE PLANNER";

}
void displayMenu(unordered_map<string, Course>& courseTable) {
    printHeader();  //prints the header

    int userChoice = 0;  //Variable to store the user's menu choice
    string fileName, courseNumber, prefix;
    vector<Course> courses;

    vector<string> courseIndex;

    //Loop until the user chooses to exit (option 9)
    while (userChoice != 9) {
        //Display menu options
        cout << "\n[1] Load Data Structure.\n";
        cout << "[2] Print Course List.\n";
        cout << "[3] Print Course.\n";
        cout << "[4] Search Courses by Prefix.\n";
        cout << "[5] Validate Prerequisites.\n";
        cout << "[9] Exit\n";

        //Get user input for menu choice
        cout << "\nWhat would you like to do? ";

        if (!(cin >> userChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        //Handle the user's choice
        switch (userChoice) {
        case 1: {
            cout << "Enter the file name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  //Clear the newline character from the input buffer
            getline(cin, fileName);  //Use getline to read the entire line including spaces

            if (fileName.empty()) {
                cout << "No file name provided." << endl;
                break;
            }

            courses = readCourseData(fileName);  //Load courses from file
            if (!courses.empty()) {
                courseTable = loadCoursesIntoCourseTable(courses);  //Load data into hash table
                courseIndex = buildSortedCourseIndex(courseTable);

                cout << "Courses loaded successfully (" << courseTable.size() << " records)." << endl;

                vector<string> cyclePath;
                if (hasPrerequisiteCycle(courseTable, cyclePath)) {
                    cout << "WARNING: Circular prerequisite detected:\n";
                    for (size_t i = 0; i < cyclePath.size(); ++i) {
                        cout << cyclePath[i];
                        if (i + 1 < cyclePath.size())
                            cout << " -> ";
                    }
                    cout << "\nFix the dataset before relying on prerequisite relationships." << endl;
                }
                else {
                    cout << "Prerequisite validation passed (no cycles detected)" << endl;
                }
            }
            else {
                cout << "Failed to load courses. Please check the file and try again." << endl;
            }
            break;

        case 2:
            cout << "\nHere is a sample schedule:\n" << endl;
            sortAndPrintCourses(courseTable);  //Print courses in alphanumeric order
            break;

        case 3:
            if (courseTable.empty()) {
                cout << "Please load data first." << endl;
                break;
            }
            cout << "What course do you want to know about? (example: CSCI301)";
            cin >> courseNumber;
            printCourseInformation(courseTable, courseNumber);  //Print course details
            break;

        //Added case 4 to allow for searching by prefix
        case 4:
            if (courseTable.empty()) {
                cout << "Please load data first." << endl;
                break;
            }
            cout << "Enter prefix (example: CSCI): ";
            cin >> prefix;

            {
                vector<string> matches = searchByPrefix(courseIndex, prefix);
                if (matches.empty()) {
                    cout << "No courses found with prefix '" << toUpper(prefix) << "'." << endl;
                }
                else {
                    cout << "Matches:\n";
                    for (const auto& id : matches) {
                        const auto it = courseTable.find(id);
                        if (it != courseTable.end()) {
                            cout << it->second.courseNumber << ", " << it->second.courseTitle << endl;
                        }
                    }
                }
            }
            break;
        //Added case 5 for checking the validity of the prereqs (ensure no cycle)
        case 5:
            if (courseTable.empty()) {
                cout << "Please load data first." << endl;
                break;
            }
            {
                vector<string> cyclePath;
                if (hasPrerequisiteCycle(courseTable, cyclePath)) {
                    cout << "Circular prerequisite detected:\n";
                    for (size_t i = 0; i < cyclePath.size(); ++i) {
                        cout << cyclePath[i];
                        if (i + 1 < cyclePath.size()) cout << " -> ";
                    }
                    cout << endl;
                }
                else {
                    cout << "No circular prerequisites detected." << endl;
                }
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;  //Exit the program
            break;

        default:
            cout << userChoice << " is not a valid option." << endl;  //Error for invalid input

        }
        }
    }
}

//Main function to start the program
int main() {
    unordered_map<string, Course> courseTable;  //Hash table to store the courses
    displayMenu(courseTable);  //Call the menu function
    return 0;  //End of program
}
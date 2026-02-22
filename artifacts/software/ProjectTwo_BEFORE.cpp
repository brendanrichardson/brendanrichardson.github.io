//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Brendan Richardson
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Course Planning Program to Store Course Information
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>

using namespace std;

//Define the Course struct to hold course data
struct Course {
    string courseNumber;           //The unique identifier for the course
    string courseTitle;            //The title of the course
    vector<string> prerequisites;  //A list of prerequisite courses
};

//Function to convert a string to uppercase
string toUpper(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
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
        vector<string> prerequisites;

        //Extract course number and title
        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');

        //Extract prerequisites if any
        while (getline(ss, prerequisite, ',')) {
            prerequisites.push_back(prerequisite);
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
    if (courseTable.find(upperCourseNumber) != courseTable.end()) {
        const Course& course = courseTable.at(upperCourseNumber);
        //Print course number and title
        cout << course.courseNumber << ", " << course.courseTitle << endl;
        //Print prerequisites or indicate that there are none
        if (course.prerequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        }
        else {
            cout << "Prerequisites: ";
            for (const auto& prerequisite : course.prerequisites) {
                cout << prerequisite << " ";
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
    for (const auto& course : coursesList) {
        courseTable[toUpper(course.courseNumber)] = course;  //Insert each course into the hash table
    }
    return courseTable;  //Return the populated hash table
}

//Function to sort and print courses in alphanumeric order
void sortAndPrintCourses(const unordered_map<string, Course>& courseTable) {
    //Check if the hash table is not empty
    if (!courseTable.empty()) {
        vector<Course> sortedCourses;  //Vector to store courses for sorting
        for (const auto& pair : courseTable) {
            sortedCourses.push_back(pair.second);  //Add each course to the vector
        }

        //Sort courses by course number
        sort(sortedCourses.begin(), sortedCourses.end(), [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
            });

        //Print each sorted course
        for (const auto& course : sortedCourses) {
            cout << course.courseNumber << ", " << course.courseTitle << endl;
        }
    }
    else {
        cout << "Course table is empty. Please load data first." << endl;  //Error if no data loaded
    }
}

//Function to display the menu and handle user choices
void displayMenu(unordered_map<string, Course>& courseTable) {
    cout << "Welcome to the course planner." << endl;  //Welcome message

    int userChoice = 0;  //Variable to store the user's menu choice
    string fileName, courseNumber;
    vector<Course> courses;

    //Loop until the user chooses to exit (option 9)
    while (userChoice != 9) {
        //Display menu options
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        //Get user input for menu choice
        cout << "What would you like to do? ";
        cin >> userChoice;

        //Handle the user's choice
        switch (userChoice) {
        case 1:
            cout << "Enter the file name: ";
            cin.ignore();  //Clear the newline character from the input buffer
            getline(cin, fileName);  //Use getline to read the entire line including spaces


            if (fileName.empty()) {
                cout << "No file name provided." << endl;
                cin.clear();  //Clear the input stream
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  //Ignore invalid input
                continue;
            }

            courses = readCourseData(fileName);  //Load courses from file
            if (!courses.empty()) {
                courseTable = loadCoursesIntoCourseTable(courses);  //Load data into hash table
                cout << "Courses loaded successfully." << endl;
            }
            else {
                cout << "Failed to load courses. Please check the file and try again." << endl;
            }
            break;
        case 2:
            cout << "Here is a sample schedule:" << endl;
            sortAndPrintCourses(courseTable);  //Print courses in alphanumeric order
            break;
        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            printCourseInformation(courseTable, courseNumber);  //Print course details
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;  //Exit the program
            break;
        default:
            cout << userChoice << " is not a valid option." << endl;  //Error for invalid input
            cin.clear();  //Clear the input stream
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  //Ignore invalid input
        }
    }
}

//Main function to start the program
int main() {
    unordered_map<string, Course> courseTable;  //Hash table to store the courses
    displayMenu(courseTable);  //Call the menu function
    return 0;  //End of program
}
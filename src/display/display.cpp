#include "display.h"
#include "course_db.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>

using namespace std;

// Global variable definitions
User currentUser;
vector<Schedule *> schedules;
CourseDatabase courseDb; // Course database object

// Member functions of the Schedule struct
Schedule::Schedule(int year, const string &semester, const string &department, int totalCredits = 0)
    : year(year), semester(semester), department(department), totalCredits(totalCredits)
{
    random_device rd;
    id = rd() % 99 + 1;
}

void Schedule::addCourse(const string &course)
{
    courses.push_back(course);
}

void Schedule::addDay(const string &day)
{
    days.push_back(day);
}

void Schedule::removeCourse(int index)
{
    if (index >= 0 && index < courses.size())
    {
        courses.erase(courses.begin() + index);
    }
}

void Schedule::removeDay(int index)
{
    if (index >= 0 && index < days.size())
    {
        days.erase(days.begin() + index);
    }
}

void Schedule::display()
{
    cout << "===========================================================\n";
    vector<string> daysOfWeek = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    vector<vector<string>> scheduleTable(7);
    unordered_set<string> addedCourses;

    for (const auto &course : courses)
    {
        for (const auto &courseObj : courseDb.query({}))
        {
            if (courseObj.get_name() == course && addedCourses.find(course) == addedCourses.end())
            {
                for (const auto &courseTime : courseObj.get_times())
                {
                    int dayIndex = static_cast<int>(courseTime.weekday);
                    if (find(days.begin(), days.end(), daysOfWeek[dayIndex]) == days.end())
                    {
                        scheduleTable[dayIndex].push_back(course);
                    }
                }
                addedCourses.insert(course);
            }
        }
    }

    for (int i = 0; i < 7; ++i)
    {
        cout << daysOfWeek[i] << " | ";
        for (const auto &course : scheduleTable[i])
        {
            cout << course << " | ";
        }
        cout << "\n";
    }
    cout << "===========================================================\n";
}

// Function definitions
void createSchedule(User &currentUser, vector<Schedule *> &schedules)
{
    int year = 0, totalCredits = 0;
    string semester;
    string department;
    vector<string> selectedCourses;
    bool isEnglishA;
    vector<string> avoidDays;

    system("cls");
    // Load course data from the database
    courseDb.load();
    vector<Course> courses = courseDb.query({});

    // Check if courses are loaded properly
    if (courses.empty())
    {
        cout << "No courses found in the database. Please check the database." << endl;
        return;
    }

    // Select year
    cout << "Enter the year for the schedule (e.g., 2023, 2024): ";
    cin >> year;
    cin.ignore(); // Clear input buffer

    system("cls");
    // Select grade level based on user setup
    cout << "You are currently in year " << currentUser.year << " (as set in User Setup). Press Enter to continue...";
    cin.ignore();

    system("cls");
    // Select semester
    int semesterChoice;
    while (true)
    {
        cout << "Select a semester:\n1. Spring\n2. Summer\n3. Fall\n4. Winter\nSelect: ";
        cin >> semesterChoice;
        cin.ignore(); // Clear input buffer
        if (semesterChoice >= 1 && semesterChoice <= 4)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please select a number between 1 and 4." << endl;
        }
    }
    switch (semesterChoice)
    {
    case 1:
        semester = "Spring";
        break;
    case 2:
        semester = "Summer";
        break;
    case 3:
        semester = "Fall";
        break;
    case 4:
        semester = "Winter";
        break;
    }

    system("cls");
    // Select department
    set<string> departments;
    for (const auto &course : courses)
    {
        for (const auto &dept : course.get_departments())
        {
            departments.insert(static_cast<string>(encode_department(dept)));
        }
    }

    if (departments.empty())
    {
        cout << "No departments found. Please check the course data." << endl;
        return;
    }

    int departmentChoice;
    vector<string> departmentList(departments.begin(), departments.end());
    while (true)
    {
        cout << "Select a department: " << endl;
        int idx = 1;
        for (const auto &dept : departmentList)
        {
            cout << idx++ << ". ComputerScience" << endl; // Display as ComputerScience
        }
        cin >> departmentChoice;
        cin.ignore(); // Clear input buffer
        if (departmentChoice >= 1 && departmentChoice <= departmentList.size())
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please select a valid department number." << endl;
        }
    }
    department = departmentList[departmentChoice - 1];

    system("cls");
    // Select course categories
    vector<string> courseCategories = {"General_Education",     "CoreCommunication",  "CoreCreativity",
                                       "CoreChallenge",         "CoreConvergence",    "CoreTrust",
                                       "ElectiveCommunication", "ElectiveCreativity", "ElectiveChallenge",
                                       "ElectiveConvergence",   "ElectiveTrust",      "Major",
                                       "MajorRequired",         "MajorFundamental",   "Next Step"};

    unordered_set<string> addedCourses;
    while (true)
    {
        system("cls");
        cout << "Select a course category: " << endl;
        int idx = 1;
        for (const auto &category : courseCategories)
        {
            cout << idx++ << ". " << category << endl;
        }
        cout << "Select: ";
        int categoryChoice;
        cin >> categoryChoice;
        cin.ignore(); // Clear input buffer

        if (categoryChoice == 15)
        {
            break;
        }
        if (categoryChoice < 1 || categoryChoice > 14)
        {
            cout << "Invalid choice. Please select a number between 1 and 15." << endl;
            continue;
        }

        CourseType selectedCategory = static_cast<CourseType>(categoryChoice - 1);
        system("cls");
        // Select course
        cout << "Select courses for category " << courseCategories[categoryChoice - 1]
             << " (you can select multiple): " << endl;
        vector<Course> categoryCourses;
        unordered_set<string> uniqueCourseNames;
        for (const auto &course : courses)
        {
            if (course.get_type() == selectedCategory &&
                encode_department(*course.get_departments().begin()) == department &&
                addedCourses.find(course.get_name()) == addedCourses.end() &&
                uniqueCourseNames.find(course.get_name()) == uniqueCourseNames.end())
            {
                categoryCourses.push_back(course);
                uniqueCourseNames.insert(course.get_name());
                cout << categoryCourses.size() << ". " << course.get_name() << endl;
            }
        }

        if (categoryCourses.empty())
        {
            cout << "No courses found for the selected category and department." << endl;
            continue;
        }

        int courseChoice;
        while (true)
        {
            cout << "> ";
            cin >> courseChoice;
            cin.ignore(); // Clear input buffer
            if (courseChoice == 0)
            {
                break;
            }
            if (courseChoice >= 1 && courseChoice <= categoryCourses.size())
            {
                Course selectedCourse = categoryCourses[courseChoice - 1];
                selectedCourses.push_back(selectedCourse.get_name());
                addedCourses.insert(selectedCourse.get_name());
            }
            else
            {
                cout << "Invalid choice. Please select a valid course number." << endl;
            }
        }
    }

    system("cls");
    // Set days to avoid scheduling classes
    cout << "Select days to avoid: " << endl;
    vector<string> daysOfWeek = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    for (int i = 0; i < daysOfWeek.size(); ++i)
    {
        cout << i + 1 << ". " << daysOfWeek[i] << endl;
    }
    cout << "Select days to avoid (enter numbers, separated by space, and type 0 to finish): ";
    while (true)
    {
        int dayChoice;
        cin >> dayChoice;
        if (dayChoice == 0)
            break;
        if (dayChoice >= 1 && dayChoice <= daysOfWeek.size())
        {
            avoidDays.push_back(daysOfWeek[dayChoice - 1]);
        }
        else
        {
            cout << "Invalid choice. Please select a valid day number." << endl;
        }
    }
    cin.ignore(); // Clear input buffer

    system("cls");
    cout << "Creating a new schedule...";
    // Create a new schedule with a random ID
    Schedule *schedule = new Schedule(year, semester, department, totalCredits);
    for (const auto &course : selectedCourses)
    {
        schedule->addCourse(course);
    }
    for (const auto &day : avoidDays)
    {
        schedule->addDay(day);
    }
    schedules.push_back(schedule);
    cout << "Schedule created. ID: " << schedule->id << endl;
    schedule->display();
    cout << "Press Enter to return to the main menu...";
    cin.ignore();
}





void searchAndModifySchedule()
{
    cout << "Schedule search and modification feature is not yet implemented." << endl;
}

void setupUser(User &user)
{
    system("cls");
    cout << "Enter user name: ";
    cin >> user.name;

    while (true)
    {
        cout << "Enter user year (1-4): ";
        cin >> user.year;
        if (user.year >= 1 && user.year <= 4)
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
        }
    }

    while (true)
    {
        cout << "Enter user student ID (8-digit number): ";
        cin >> user.student_id;
        if (user.student_id >= 10000000 && user.student_id <= 99999999)
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter an 8-digit number." << endl;
        }
    }
    cout << "Enter user department: ";
    cin >> user.department;
    cin.ignore(); // Clear input buffer
}
void mainMenu()
{
    system("cls");
    int choice;

    while (true)
    {
        system("cls");
        cout << "[MainMenu]\n1. User Setup\n2. Create Schedule\n3. Search and Modify Schedule\n4. Exit\nSelect: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice)
        {
        case 1:
            system("cls");
            setupUser(currentUser);
            break;
        case 2:
            system("cls");
            createSchedule(currentUser, schedules);
            break;
        case 3:
            system("cls");
            searchAndModifySchedule();
            break;
        case 4:
            system("cls");
            cout << "Exiting the program." << endl;
            return;
        default:
            cout << "Please select a valid option." << endl;
        }
    }
}

#include "display.h"
#include "course_db.h"
#include <algorithm>
#include <iostream>

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
    cout << "=====================================================================================================================================\n";
    cout << "      Mon        |        Tue       |        Wed       |        Thu       |        Fri       |        Sat       |        Sun "
         << endl;
    vector<vector<string>> scheduleTable(7);
    for (const auto &course : courses)
    {
        // Add courses by day (in actual implementation, proper day info is needed)
        scheduleTable[0].push_back(course); // Assigned to Monday as an example
    }
    for (int i = 0; i < 7; ++i)
    {
        cout << "\n";
        for (const auto &course : scheduleTable[i])
        {
            cout << course << "    | ";
        }
    }
    cout << "\n=======================================================================================================" << endl;
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
    string day;

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
    // Select course category
    vector<string> courseCategories = {
        "General_Education",     // 공통교양
        "CoreCommunication",     // 핵심-소통
        "CoreCreativity",        // 핵심-창의
        "CoreChallenge",         // 핵심-도전
        "CoreConvergence",       // 핵심-융합
        "CoreTrust",             // 핵심-신뢰
        "ElectiveCommunication", // 선택-소통
        "ElectiveCreativity",    // 선택-창의
        "ElectiveChallenge",     // 선택-도전
        "ElectiveConvergence",   // 선택-융합
        "ElectiveTrust",         // 선택-신뢰
        "Major",                 // 전공
        "MajorRequired",         // 전공필수
        "MajorFundamental",      // 전공기초
        "Next Step"              // 다음 단계
    };

    while (true)
    {
        system("cls");
        cout << "Select a course category: " << endl;
        int idx = 1;
        for (const auto &category : courseCategories)
        {
            cout << idx++ << ". " << category << endl;
        }
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
        cout << "Select a course: " << endl;
        vector<Course> categoryCourses;
        for (const auto &course : courses)
        {
            if (course.get_type() == selectedCategory &&
                encode_department(*course.get_departments().begin()) == department)
            {
                categoryCourses.push_back(course);
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
            cin >> courseChoice;
            cin.ignore(); // Clear input buffer
            if (courseChoice >= 1 && courseChoice <= categoryCourses.size())
            {
                break;
            }
            else
            {
                cout << "Invalid choice. Please select a valid course number." << endl;
            }
        }
        Course selectedCourse = categoryCourses[courseChoice - 1];
        selectedCourses.push_back(selectedCourse.get_name());
    }

    system("cls");
    // Choose if it's an English A course
    cout << "Is this an English A course? (1: Yes, 0: No): ";
    while (true)
    {
        cin >> isEnglishA;
        cin.ignore(); // Clear input buffer
        if (isEnglishA == 0 || isEnglishA == 1)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 1 for Yes or 0 for No." << endl;
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

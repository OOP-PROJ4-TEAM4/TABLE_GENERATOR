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
Schedule::Schedule(int year, const string &semester, const string &department, int totalCredits)
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
    cout << "      Mon        |        Tue       |        Wed       |        Thu       |        Fri       |        Sat "
            "      |        Sun "
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
    cout << "\n===========================================================" << endl;
}

// Function definitions
void createSchedule(User &currentUser, vector<Schedule *> &schedules)
{
    int year, totalCredits;
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
    system("cls");
    set<int> years;
    for (const auto &course : courses)
    {
        years.insert(course.get_year());
    }

    cout << "Select a year: " << endl;
    int idx = 1;
    vector<int> yearList(years.begin(), years.end());
    for (const auto &yr : yearList)
    {
        cout << idx++ << ". " << yr << endl;
    }
    int yearChoice;
    cin >> yearChoice;
    cin.ignore(); // Clear input buffer
    year = yearList[yearChoice - 1];

    system("cls");
    // Select semester
    set<string> semesters;
    for (const auto &course : courses)
    {
        semesters.insert(encode_semester(course.get_semester()));
    }

    cout << "Select a semester: " << endl;
    idx = 1;
    vector<string> semesterList(semesters.begin(), semesters.end());
    for (const auto &sem : semesterList)
    {
        cout << idx++ << ". " << sem << endl;
    }
    int semesterChoice;
    cin >> semesterChoice;
    cin.ignore(); // Clear input buffer
    semester = semesterList[semesterChoice - 1];

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

    cout << "Select a department: " << endl;
    idx = 1;
    vector<string> departmentList(departments.begin(), departments.end());
    for (const auto &dept : departmentList)
    {
        cout << idx++ << ". " << dept << endl;
    }
    int departmentChoice;
    cin >> departmentChoice;
    cin.ignore(); // Clear input buffer
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
        idx = 1;
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
        cin >> courseChoice;
        cin.ignore(); // Clear input buffer
        Course selectedCourse = categoryCourses[courseChoice - 1];
        selectedCourses.push_back(selectedCourse.get_name());
    }

    system("cls");
    // Choose if it's an English A course
    cout << "Is this an English A course? (1: Yes, 0: No): ";
    cin >> isEnglishA;
    cin.ignore(); // Clear input buffer

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
    }
    cin.ignore(); // Clear input buffer

    system("cls");
    // Create a new schedule with a random ID
    Schedule *schedule = new Schedule(year, semester, department, totalCredits);
    for (const auto &course : selectedCourses)
    {
        schedule->addCourse(course);
    }
    schedules.push_back(schedule);
    cout << "Schedule created. ID: " << schedule->id << endl;
    schedule->display();
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
    cout << "Enter user year: ";
    cin >> user.year;
    cout << "Enter user student ID: ";
    cin >> user.student_id;
    cout << "Enter user department: ";
    cin >> user.department;
    cin.ignore(); // Clear input buffer
}

void mainMenu()
{
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

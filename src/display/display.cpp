#include "display.h"
#include "course.h"
#include "course_db.h"
#include "display_helper.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_set>

using namespace std;

vector<Schedule *> schedules;
CourseDatabase courseDb;

Display::Display(CourseDatabase &courseDatabase, TableDatabase &tableDatabase, TableGenerator &tableGenerator)
    : courseDatabase(courseDatabase), tableDatabase(tableDatabase), tableGenerator(tableGenerator)
{
}

void Display::display()
{
    std::vector<ColumnItem> columns = {ColumnItem{"1"}, ColumnItem{"2"}, ColumnItem{"3"}, ColumnItem{"4"},
                                       ColumnItem{"5"}, ColumnItem{"6"}, ColumnItem{"7"}, ColumnItem{"8"},
                                       ColumnItem{"9"}, ColumnItem{"10"}};

    std::map<Weekday, RowItem> rows;
    rows[Weekday::Sun] = RowItem{"Sun", std::vector<std::string>(10, "")};
    rows[Weekday::Mon] = RowItem{"Mon", std::vector<std::string>(10, "")};
    rows[Weekday::Tue] = RowItem{"Tue", std::vector<std::string>(10, "")};
    rows[Weekday::Wed] = RowItem{"Wed", std::vector<std::string>(10, "")};
    rows[Weekday::Thu] = RowItem{"Thu", std::vector<std::string>(10, "")};
    rows[Weekday::Fri] = RowItem{"Fri", std::vector<std::string>(10, "")};
    rows[Weekday::Sat] = RowItem{"Sat", std::vector<std::string>(10, "")};
    for (const auto &course : table->get_course())
    {
        for (const auto &time : course.get_times())
        {
            rows[time.weekday].columns[time.time - 1] = course.get_name();
        }
    }
    draw_table("Timetable", columns,
               {rows[Weekday::Sun], rows[Weekday::Mon], rows[Weekday::Tue], rows[Weekday::Wed], rows[Weekday::Thu],
                rows[Weekday::Fri], rows[Weekday::Sat]});
}

void Display::searchSchedule()
{
    std::vector<Table> tables = tableDatabase.get_tables();
    std::optional<Table> selected_table;
    do
    {
        selected_table = select_one(
            tables, [](Table table) { return "Scehdule "; }, "Select schedule");
        if (selected_table.has_value())
        {
            table = &selected_table.value();
            display();
        }
    } while (selected_table.has_value());
}

void Display::createSchedule()
{
    system("cls");
    vector<Course> courses = courseDatabase.query({});
    if (courses.empty())
    {
        get_input("No courses found in the database. Please check the database");
        return;
    }

    CourseQuery query;

    table = new Table();

    // 년도 설정
    std::optional<int> selected_year;
    do
    {
        selected_year = select_one(
            std::vector<int>{2024}, [](int item) { return std::to_string(item); }, "Enter the year for the schedule.");
    } while (!selected_year.has_value());

    if (selected_year.has_value())
        query.year = selected_year.value();
    query.user_year = user.year;

    system("cls");
    std::string year_alert = "You are currently in year " + std::to_string(user.year) +
                             " (as set in User Setup). Press Enter to continue...";

    // 학기 설정
    std::optional<Semester> selected_semester;
    do
    {
        selected_semester = select_one(
            std::vector<Semester>{Semester::Spring, Semester::Summer, Semester::Fall, Semester::Winter},
            [](Semester item) {
                switch (item)
                {
                case Semester::Spring:
                    return "Spring";
                case Semester::Summer:
                    return "Summer";
                case Semester::Fall:
                    return "Fall";
                case Semester::Winter:
                    return "Winter";
                }
            },
            "Select a semester");
    } while (!selected_semester.has_value());
    if (selected_semester.has_value())
        query.semester = selected_semester.value();

    // 부서 설정
    query.departments = {user.department};

    // 교수 설정
    std::vector<std::string> professor_list = {};
    std::set<std::string> professor_set = {};

    for (const auto &course : courses)
    {
        if (professor_set.count(course.get_professor()) == 0)
        {
            professor_list.push_back(course.get_professor());
            professor_set.insert(course.get_professor());
        }
    }

    professor_list[0] = "손봉수";


    std::vector<std::string> selected_professors = {};
    selected_professors = select_multiple(
        professor_list, [](std::string item) { return item; }, 10,
        "Select a preferred professor. You can add multiple professor.");
    query.professors.insert(selected_professors.begin(), selected_professors.end());

    // 요일 설정
    std::vector<Weekday> selected_weekdays = select_multiple(
        std::vector<Weekday>{Weekday::Sun, Weekday::Mon, Weekday::Tue, Weekday::Wed, Weekday::Thu, Weekday::Fri,
                             Weekday::Sat},
        [](Weekday item) {
            switch (item)
            {
            case Weekday::Sun:
                return "Sun";
            case Weekday::Mon:
                return "Mon";
            case Weekday::Tue:
                return "Tue";
            case Weekday::Wed:
                return "Wed";
            case Weekday::Thu:
                return "Thu";
            case Weekday::Fri:
                return "Fri";
            case Weekday::Sat:
                return "Sat";
            }
        },
        7, "Select the day(s) you prefer.");
    query.weekdays.insert(selected_weekdays.begin(), selected_weekdays.end());

    int selected_credit;
    do
    {
        system("cls");
        selected_credit = get_integer_input("Total credit? (Max 23)");
    } while (selected_credit < 1 || selected_credit > 23);

    tableGenerator.setTotalGrade(selected_credit);
    tableGenerator.setQuery(query);
    tableGenerator.generateTable(*table);
    tableDatabase.insert(*table);
    tableDatabase.save();
    display();
}

void Display::searchCourse()
{

    CourseQuery query;

    query.user_year = user.year;

    std::optional<int> selected_year;
    do
    {
        selected_year = select_one(
            std::vector<int>{2024}, [](int item) { return std::to_string(item); }, "Enter the year for the schedule.");
    } while (!selected_year.has_value());

    if (selected_year.has_value())
        query.year = selected_year.value();

    // 학기 설정
    std::optional<Semester> selected_semester;
    do
    {
        selected_semester = select_one(
            std::vector<Semester>{Semester::Spring, Semester::Summer, Semester::Fall, Semester::Winter},
            [](Semester item) {
                switch (item)
                {
                case Semester::Spring:
                    return "Spring";
                case Semester::Summer:
                    return "Summer";
                case Semester::Fall:
                    return "Fall";
                case Semester::Winter:
                    return "Winter";
                }
            },
            "Select a semester");
    } while (!selected_semester.has_value());
    if (selected_semester.has_value())
        query.semester = selected_semester.value();

    std::string title;
    std::vector<ColumnItem> columns = {ColumnItem{"Course"}, ColumnItem{"Professor"}, ColumnItem{"Grade"},
                                       ColumnItem{"Time"}};
    do
    {
        system("cls");
        title = get_input("Enter course name (or quit): ");
        if (title == "quit")
            return;
        query.name = title;

        vector<Course> result = courseDatabase.query(query);
        std::string table_title;
        table_title = "Found " + std::to_string(result.size()) + " courses named \"" + query.name + "\"";
        std::vector<RowItem> rows;
        int index = 0;
        for (const auto &course : result)
        {
            index++;
            RowItem row = {std::to_string(index), {}};
            row.columns.push_back(course.get_name());
            row.columns.push_back(course.get_professor());
            row.columns.push_back(std::to_string(course.get_grade()));
            std::string time_str;
            for (const auto &time : course.get_times())
            {
                switch (time.weekday)
                {
                case Weekday::Mon:
                    time_str += "Mon-";
                    break;
                case Weekday::Tue:
                    time_str += "Tue-";
                    break;
                case Weekday::Wed:
                    time_str += "Wed-";
                    break;
                case Weekday::Thu:
                    time_str += "Thu-";
                    break;
                case Weekday::Fri:
                    time_str += "Fri-";
                    break;
                case Weekday::Sat:
                    time_str += "Sat-";
                    break;
                case Weekday::Sun:
                    time_str += "Sun-";
                    break;
                }
                time_str += std::to_string(time.time);
                time_str += " ";
            }
            row.columns.push_back(time_str);
            rows.push_back(row);
        }
        draw_table(table_title, columns, rows);
    } while (title != "quit");
}

void Display::setupUser()
{
    system("cls");

    user.name = get_input("Enter user name: ");
    do
    {
        user.year = get_integer_input("Enter user year (1-4): ");
    } while (user.year < 1 || user.year > 4);

    do
    {
        user.student_id = get_integer_input("Enter user student ID (8-digit number): ");
    } while (user.student_id < 10000000 && user.student_id > 99999999);

    std::vector<Department> selectable_departments = {Department::ComputerScience};
    std::optional<Department> department = select_one(
        selectable_departments,
        [](Department item) {
            switch (item)
            {
            case Department::ComputerScience:
                return "Computer Science";
            }
        },
        "Select department");
    if (department)
        user.department = department.value();
}

void Display::mainMenu()
{
    while (true)
    {
        std::optional<int> choice = select_one(
            std::vector<int>{1, 2, 3, 4, 5},
            [](int menu) {
                switch (menu)
                {
                case 1:
                    return "User setup";
                case 2:
                    return "Create schedule";
                case 3:
                    return "Search schedule";
                case 4:
                    return "Search course";
                case 5:
                    return "Quit";
                default:
                    return "";
                }
            },
            "Menu");
        if (choice.has_value())
        {
            switch (choice.value())
            {
            case 1:
                system("cls");
                setupUser();
                break;
            case 2:
                system("cls");
                createSchedule();
                break;
            case 3:
                system("cls");
                searchSchedule();
                break;
            case 4:
                system("cls");
                searchCourse();
                break;
            case 5:
                return;
            default:
                cout << "Please select a valid option." << endl;
            }
        }
    }
}

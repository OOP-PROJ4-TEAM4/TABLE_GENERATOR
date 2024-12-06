#ifndef DISPLAY_H
#define DISPLAY_H

#include "course.h"          // A의 헤더 파일을 포함
#include "course_db.h"       // A의 헤더 파일을 포함
#include "table.h"           // A의 헤더 파일을 포함
#include "table_db.h"        // A의 헤더 파일을 포함
#include "table_generator.h" // A의 헤더 파일을 포함
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// 사용자 정보 구조체
struct User
{
    string name;
    int year;
    int student_id;
    string department;
};

// 전역 사용자 및 시간표 데이터
extern User currentUser;
extern vector<class Schedule *> schedules;

// Schedule 구조체 정의
struct Schedule
{
    int id;
    int year;
    string semester;
    string department;
    int totalCredits;
    vector<string> courses;
    vector<string> days;

    Schedule(int year, const string &semester, const string &department, int totalCredits);
    void addCourse(const string &course);
    void addDay(const string &day);
    void removeCourse(int index);
    void removeDay(int index);
    void display();
};

// 함수 선언
void createSchedule(User &currentUser, vector<Schedule *> &schedules);
void searchAndModifySchedule();
void setupUser(User &user);
void mainMenu();

#endif // DISPLAY_H

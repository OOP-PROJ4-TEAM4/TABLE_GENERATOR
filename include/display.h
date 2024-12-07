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
#include <unordered_set>
#include <vector>

using namespace std;

// 전방 선언 (forward declaration)
struct Schedule;

// 사용자 정보 구조체
struct User
{
    string name;
    int year;
    int student_id;
    string department;
};

// 전역 사용자 및 시간표 데이터 (선언만)
extern User currentUser;
extern vector<Schedule *> schedules;       // 사용자 시간표 목록
extern vector<string> selectedProfessors;  // 사용자 선택 교수 목록
extern bool isEnglishA;                    // 사용자 영어 A 과목 여부
extern int maxCredits;                     // 사용자 최대 학점
extern unordered_set<string> selectedDays; // 사용자 선택 요일 목록

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

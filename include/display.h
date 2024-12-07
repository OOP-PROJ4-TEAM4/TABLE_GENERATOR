#ifndef DISPLAY_H
#define DISPLAY_H

#include "course.h"          
#include "course_db.h"       
#include "table.h"          
#include "table_db.h"       
#include "table_generator.h" 
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;


struct Schedule;


struct User
{
    string name;
    int year;
    int student_id;
    string department;
};


extern User currentUser;
extern vector<Schedule *> schedules;       // 사용자 시간표 목록
extern vector<string> selectedProfessors;  // 사용자 선택 교수 목록
extern bool isEnglishA;                    // 사용자 영어 A 과목 여부
extern int maxCredits;                     // 사용자 최대 학점
extern unordered_set<string> selectedDays; // 사용자 선택 요일 목록


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
    void display() const;
};


void createSchedule(User &currentUser, vector<Schedule *> &schedules);
void searchAndModifySchedule();
void setupUser(User &user);
void mainMenu();

#endif 

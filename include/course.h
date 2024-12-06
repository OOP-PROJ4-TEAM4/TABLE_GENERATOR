#ifndef COURSE_H
#define COURSE_H

#include <optional>
#include <set>
#include <string>
#include <vector>

typedef int Time;

enum class Weekday
{
    Sun,
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
    Sat
};

enum class CourseType
{
    General_Education,     // 공통교양
    CoreCommunication,     // 핵심-소통
    CoreCreativity,        // 핵심-창의
    CoreChallenge,         // 핵심-도전
    CoreConvergence,       // 핵심-융합
    CoreTrust,             // 핵심-신뢰
    ElectiveCommunication, // 선택-소통
    ElectiveCreativity,    // 선택-창의
    ElectiveChallenge,     // 선택-도전
    ElectiveConvergence,   // 선택-융합
    ElectiveTrust,         // 선택-신뢰
    Major,                 // 전공
    MajorRequired,         // 전공필수
    MajorFundamental       // 전공기초
};

enum class Department
{
    ComputerScience
};

enum class Semester
{
    Spring,
    Summer,
    Fall,
    Winter
};

struct CourseTime
{
    Weekday weekday;
    Time time;
    std::string room;
    std::string building;

    bool operator==(const CourseTime &) const;
};

class Course
{
    int id;
    std::string name;
    CourseType type;
    int year;
    Semester semester;
    bool english_a;
    std::string professor;
    std::set<Department> departments;
    std::vector<CourseTime> times;
    int grade;
    int minimum_year;

  public:
    Course();
    // Decode from string
    Course(const std::string &);
    // Encode to string
    std::string encode() const;

    int get_id() const;
    // 학점수를 얻습니다.
    int get_grade() const;
    // 이수가능한 최소학년입니다. 0이면 모든 학년이 이수가능합니다.
    int get_minimum_year() const;
    std::vector<CourseTime> get_times() const;
    std::string get_name() const;
    std::string get_professor() const;
    // 수강가능한 부서를 얻습니다.
    std::set<Department> get_departments() const;
    CourseType get_type() const;
    // 강의가 열리는 학기입니다.
    Semester get_semester() const;
    // 강의가 열리는 연도입니다.
    int get_year() const;
    bool is_english_a() const;

    bool operator==(const Course &) const;
};

// if fail to decode, error will be thrown
CourseTime decode_coursetime(const std::string &);
std::string encode_coursetime(const CourseTime &);

// if fail to decode, error will be thrown
Semester decode_semester(const std::string &);
std::string encode_semester(Semester);

// if fail to decode, error will be thrown
Weekday decode_weekday(const std::string &);
std::string encode_weekday(Weekday);

// if fail to decode, error will be thrown
CourseType decode_coursetype(const std::string &);
std::string encode_coursetype(const CourseType);

// if fail to decode, error will be thrown
Department decode_department(const std::string &);
std::string encode_department(const Department);
#endif
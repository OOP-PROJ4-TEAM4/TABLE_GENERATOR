#include "table_generator.h"
#include "table_db.h"
#include <vector>
#include "course_db.h"

using namespace std;

TableGenerator::TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB)
    : courseDB(courseDB), tableDB(tableDB), totalGrade(0)
{
}


void TableGenerator::generateTable(TableDatabase &tableDB, Table table) //query로 처리된 courseDB에 있는 course중 greedyAloghrithm으로 시간표 생성
{
    vector<Course> courses = this->courseDB.query(this->query);
    for (Course &course : courses) // course의 수만큼 반복
    {
        if (totalGrade >= currentGrade + course.get_times().size()) // 설정한 총 학점 수 보다 현재 학점 + course의 학점이 더 작거나 같을 경우
        {
            if (this->findTime(course)) // 시간이 겹치지 않을 경우
            {
                table.insert_course(course);                     // 시간표에 강의 추가
                this->currentGrade += course.get_times().size(); // 현재 총 학점수를 강의의 학점만큼 증가
            }
        }    
    }
    tableDB.insert(table);
}

void TableGenerator::setQuery(CourseQuery query) // table generator 생성
{
    this->query = query;
}

void TableGenerator::setTotalGrade(int grade) // 생성할 시간표의 총 학점수 설정
{
    this->totalGrade = grade;
}

bool TableGenerator::findTime(Course &course) // 겹치는 시간 있는지 확인
{
    for (const auto availableTime : course.get_times())
    {
        if (this->time.find(availableTime) != this->time.end())
        {
            return false;
        }
    }

    for (const auto availableTime : course.get_times())
    {
        this->time.insert(availableTime);
    }
    return true;
}
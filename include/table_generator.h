#ifndef TABLE_GENERATOR_H
#define TABLE_GENERATOR_H

#include <set>
#include <string>
#include "course_db.h"
#include "table_db.h"
#include <unordered_set>

using namespace std;


class TableGenerator
{
  private:
    CourseQuery query;
    CourseDatabase &courseDB; 
    TableDatabase &tableDB;

    int currentGrade = 0; // 지금까지 넣은 강의의 총 학점수
    int totalGrade; // 설정한 총 학점수
    unordered_set<CourseTime> time; // 생성한 시간표에 채워진 시간

    bool findTime(Course &course); // 겹치는 시간있는지 확인하는 함수
  public:
    void setTotalGrade(int i); // 총 학점수 설정

    void setQuery(CourseQuery query); // 쿼리 설정 공강, 학년, 학기등을 설정합니다ㅣ.

    TableGenerator(CourseDatabase &courseDB, TableDatabase &tableDB); // 생성자 courseDB와 tableDB를 받습니다.

    void generateTable(TableDatabase &tableDB, Table table); // 시간표를 만듭니다.
};
#endif
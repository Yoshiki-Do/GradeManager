#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>

#include "sqlite3.h"
#include "Assessment.h"
#include "Course.h"

using namespace std;

class DatabaseManager {
private:
	sqlite3* database;

public:
	DatabaseManager(const string& databaseFile);
	~DatabaseManager();

	void createTables();

	bool addCourse(const string& courseNumber, double passingGrade);
	bool addAssessment(const string& courseNumber, const string& assessmentName, double weight);
	bool addEvaluation(const string& courseNumber, const string& assessmentName, const string& evaluationName, double earnedScore, double possibleScore);

	bool deleteCourse(const string& courseNumber);
	bool deleteAssessment(const string& courseNumber, const string& assessmentName);
	bool deleteEvaluation(const string& courseNumber, const string& assessmentName, const string& evaluationName);


	vector<Course> loadCourses();
	vector<Assessment> loadAssessments(const string& courseNumber);
	vector<Evaluation> loadEvaluations(const string& courseNumber, const string& assessmentName);
};

#endif
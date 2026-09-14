#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <iostream>

#include "Assessment.h"

using namespace std;
class Course {
private:
	string courseNumber;
	vector<Assessment> assessments;

	double currentGrade;
	const double passingGrade = 50.0;

public:
	Course(const string& courseNumber, double passingGrade);

	string getCourseNumber()const;
	double getPassingGrade()const;
	bool isPassing()const;

	const vector<Assessment>& getAssessments()const;
	double getCurrentGrade()const;

	void addAssessment(const string& assessmentName, double weight);
	void addEvaluation(const string& assessmentName, const string& evaluationName, double earnedScore, double possibleScore);

	void calculateGrade();

	friend ostream& operator<<(ostream&, const Course&);

};

#endif
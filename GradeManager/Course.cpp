#include "Course.h"

#include <iomanip>

Course::Course(const string& courseNumber, double passingGrade)
	:courseNumber(courseNumber), currentGrade(0.0), passingGrade(passingGrade) {
}

string Course::getCourseNumber() const {
	return courseNumber;
}

double Course::getPassingGrade() const {
	return passingGrade;
}

bool Course::isPassing() const {
	return currentGrade >= passingGrade;
}

const vector<Assessment>& Course::getAssessments() const {
	return assessments;
}

double Course::getCurrentGrade() const {
	return currentGrade;
}

void Course::addAssessment(const string& assessmentName, double weight) {
	assessments.emplace_back(assessmentName, weight);
}

void Course::addEvaluation(const string& assessmentName, const string& evaluationName, double earnedScore, double possibleScore) {
	for (Assessment& assessment : assessments) {
		if (assessment.getName() == assessmentName) {
			assessment.addEvaluation(evaluationName, earnedScore, possibleScore);
			return;
		}
	}
}

void Course::calculateGrade() {
	currentGrade = 0.0;

	for (const Assessment& assessment : assessments) {
		currentGrade += assessment.getWeightedScore();
	}
}
ostream& operator<<(ostream& out, const Course& course) {
	out << fixed << setprecision(2);

	out << "Course: " << course.courseNumber << endl;
	out << "Current grade: " << course.currentGrade << "%" << endl;

	if (course.isPassing()) {
		out << "Status: Passing" << endl;
	}
	else {
		out << "Status: Failing" << endl;
	}

	for (const Assessment& assessment : course.assessments) {
		out << "\t" << assessment.getName() << " " << assessment.getWeight() << "% (" << assessment.getAveragePercentage() * 100 << "%)" << endl;

		for (const Evaluation& evaluation : assessment.getEvaluations()) {
			out << "\t\t" << evaluation.getName() << " " << evaluation.getEarnedScore() << "/" << evaluation.getPossibleScore() << endl;
		}
	}

	return out;

}

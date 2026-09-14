#include <iostream>
#include <string>
#include "Menu.h"

using namespace std;

void showMenu() {
	cout << endl;
	cout << "===== Grade Manager =====" << endl;
	cout << "1. View Courses" << endl;
	cout << "2. Add Course" << endl;
	cout << "3. Add Assessment" << endl;
	cout << "4. Add Evaluation" << endl;
	cout << "5. Delete Course" << endl;
	cout << "6. Delete Assessment" << endl;
	cout << "7. Delete Evaluation" << endl;
	cout << "0. Exit" << endl;
}

bool handleMenuChoice(DatabaseManager& database, int choice) {
	switch (choice) {

	case 0:
		return false;

	case 1: {
		viewCourses(database);
		break;
	}

	case 2: {
		addCourse(database);
		break;
	}

	case 3: {
		addAssessment(database);
		break;
	}

	case 4: {
		addEvaluation(database);
		break;
	}

	case 5: {
		deleteCourse(database);
		break;
	}

	case 6: {
		deleteAssessment(database);
		break;
	}

	case 7: {
		deleteEvaluation(database);
		break;
	}

	default:
		cout << "Invalid choice." << endl;
		break;
	}

	return true;
}

void viewCourses(DatabaseManager& database) {
	vector<Course> courses = database.loadCourses();

	for (Course& course : courses) {
		cout << course << endl;
	}
}

void addCourse(DatabaseManager& database) {
	string courseNumber;
	double passingGrade;

	cin.ignore();

	cout << "Course number: ";
	getline(cin, courseNumber);

	cout << "Passing grade: ";
	if (!inputDouble(passingGrade)) {
		cout << "Invalid input." << endl;
		return;
	}
	if (passingGrade < 0 || passingGrade >= 100) {
		cout << "Passing grade must be between 0 and 100." << endl;
		return;
	}

	if (database.addCourse(courseNumber, passingGrade))
		cout << "Course added." << endl;
	else
		cout << "Failed to add course." << endl;
}

void addAssessment(DatabaseManager& database) {
	string courseNumber;
	string assessmentName;
	double weight;

	cin.ignore();

	cout << "Course number: ";
	getline(cin, courseNumber);

	cout << "Assessment name: ";
	getline(cin, assessmentName);

	cout << "Weight: ";
	if (!inputDouble(weight)) {
		cout << "Invalid input." << endl;
		return;
	}
	if (weight < 0 || weight >= 100) {
		cout << "Weight must be between 0 and 100." << endl;
		return;
	}

	if (database.addAssessment(courseNumber, assessmentName, weight))
		cout << "Assessment added." << endl;
	else
		cout << "Failed to add assessment." << endl;
}

void addEvaluation(DatabaseManager& database) {
	string courseNumber;
	string assessmentName;
	string evaluationName;
	double earnedScore;
	double possibleScore;

	cin.ignore();

	cout << "Course number: ";
	getline(cin, courseNumber);

	cout << "Assessment name: ";
	getline(cin, assessmentName);

	cout << "Evaluation name: ";
	getline(cin, evaluationName);

	cout << "Earned score: ";
	if (!inputDouble(earnedScore)) {
		cout << "Invalid input." << endl;
		return;
	}

	cout << "Possible score: ";
	if (!inputDouble(possibleScore)) {
		cout << "Invalid input." << endl;
		return;
	}
	if (possibleScore <= 0) {
		cout << "Possible score must be greater than earned score." << endl;
	}
	if (earnedScore<0 || earnedScore > possibleScore) {
		cout << "Earned score must be between 0 and possible score." << endl;
	}

	if (database.addEvaluation(courseNumber, assessmentName, evaluationName, earnedScore, possibleScore))
		cout << "Evaluation added." << endl;
	else
		cout << "Failed to add evaluation." << endl;
}

void deleteCourse(DatabaseManager& database) {
	string courseNumber;

	cin.ignore();

	cout << "Course number: ";
	getline(cin, courseNumber);

	if (database.deleteCourse(courseNumber))
		cout << "Course deleted." << endl;
	else
		cout << "Failed to delete course." << endl;
}

void deleteAssessment(DatabaseManager& database) {
	string courseNumber;
	string assessmentName;

	cin.ignore();

	cout << "Course number: ";
	getline(cin, courseNumber);

	cout << "Assessment name: ";
	getline(cin, assessmentName);

	if (database.deleteAssessment(courseNumber, assessmentName))
		cout << "Assessment deleted." << endl;
	else
		cout << "Failed to delete assessment." << endl;
}

void deleteEvaluation(DatabaseManager& database) {
	string courseNumber;
	string assessmentName;
	string evaluationName;

	cin.ignore();

	cout << "Course number: ";
	getline(cin, courseNumber);

	cout << "Assessment name: ";
	getline(cin, assessmentName);

	cout << "Evaluation name: ";
	getline(cin, evaluationName);

	if (database.deleteEvaluation(courseNumber, assessmentName, evaluationName))
		cout << "Evaluation deleted." << endl;
	else
		cout << "Failed to delete evaluation." << endl;
}

bool inputDouble(double& value) {
	cin >> value;

	if (cin.fail()) {
		cin.clear();
		cin.ignore(1000, '\n');
		return false;
	}

	return true;
}

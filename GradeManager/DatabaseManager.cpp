#include "DatabaseManager.h"

#include <iostream>

DatabaseManager::DatabaseManager(const string& databaseFile) :database(nullptr) {
	int result = sqlite3_open(databaseFile.c_str(), &database);

	if (result != SQLITE_OK) {
		cerr << "Failed to open database: " << sqlite3_errmsg(database) << endl;

		sqlite3_close(database);
		database = nullptr;
	}

	sqlite3_exec(database, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

}

DatabaseManager::~DatabaseManager() {
	if (database != nullptr) {
		sqlite3_close(database);
		database = nullptr;
	}
}

void DatabaseManager::createTables() {
	const char* sql = R"(
		CREATE TABLE IF NOT EXISTS courses (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			course_number TEXT NOT NULL UNIQUE,
			passing_grade REAL NOT NULL DEFAULT 50.0
		);

		CREATE TABLE IF NOT EXISTS assessments (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			course_id INTEGER NOT NULL,
			assessment_name TEXT NOT NULL,
			weight REAL NOT NULL,
			UNIQUE(course_id, assessment_name),
			FOREIGN KEY(course_id) REFERENCES courses(id) ON DELETE CASCADE
		);

		 CREATE TABLE IF NOT EXISTS evaluations (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			assessment_id INTEGER NOT NULL,
			evaluation_name TEXT NOT NULL,
			earned_score REAL,
			possible REAL NOT NULL,
			UNIQUE(assessment_id, evaluation_name),
			FOREIGN KEY(assessment_id) REFERENCES assessments(id) ON DELETE CASCADE
		);
	)";

	char* errorMessage = nullptr;

	int result = sqlite3_exec(database, sql, nullptr, nullptr, &errorMessage);

	if (result != SQLITE_OK) {
		cerr << "Failed to create tables: " << errorMessage << endl;

		sqlite3_free(errorMessage);
	}
}

bool DatabaseManager::addCourse(const string& courseNumber, double passingGrade) {
	const char* sql = R"(INSERT OR IGNORE INTO courses (course_number, passing_grade) VALUES(?, ?);)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to prepare course insertion." << endl;
		return false;
	}

	sqlite3_bind_text(statement, 1, courseNumber.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_double(statement, 2, passingGrade);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE) {
		cerr << "Failed to add course: " << sqlite3_errmsg(database) << endl;

		sqlite3_finalize(statement);
		return false;
	}

	sqlite3_finalize(statement);

	return true;
}

bool DatabaseManager::addAssessment(const string& courseNumber, const string& assessmentName, double weight) {
	const char* sql = R"(
		INSERT INTO assessments(course_id, assessment_name, weight)
		SELECT id, ?, ? FROM courses WHERE course_number = ?
		ON CONFLICT(course_id, assessment_name)
		DO UPDATE SET weight = excluded.weight;
	)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to prepare assessment insertion." << endl;
		return false;
	}

	sqlite3_bind_text(statement, 1, assessmentName.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_double(statement, 2, weight);
	sqlite3_bind_text(statement, 3, courseNumber.c_str(), -1, SQLITE_TRANSIENT);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE) {
		cerr << "Failed to add assessment: " << sqlite3_errmsg(database) << endl;

		sqlite3_finalize(statement);
		return false;
	}

	if (sqlite3_changes(database) == 0) {
		sqlite3_finalize(statement);
		return false;
	}

	sqlite3_finalize(statement);
	return true;
}

bool DatabaseManager::addEvaluation(const string& courseNumber, const string& assessmentName, const string& evaluationName, double earnedScore, double possibleScore) {
	const char* sql = R"(
		INSERT INTO evaluations(
			assessment_id, evaluation_name, earned_score, possible
		)
		SELECT assessments.id, ?, ?, ?
		FROM assessments
		INNER JOIN courses
			ON assessments.course_id = courses.id
		WHERE courses.course_number = ?
			AND assessments.assessment_name = ?
		ON CONFLICT(assessment_id, evaluation_name)
		DO UPDATE SET
			earned_score = excluded.earned_score,
			possible = excluded.possible;
	)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to prepare evaluation insertion: "
			<< sqlite3_errmsg(database) << endl;
		return false;
	}

	sqlite3_bind_text(statement, 1, evaluationName.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_double(statement, 2, earnedScore);
	sqlite3_bind_double(statement, 3, possibleScore);
	sqlite3_bind_text(statement, 4, courseNumber.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(statement, 5, assessmentName.c_str(), -1, SQLITE_TRANSIENT);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE) {
		cerr << "Failed to add evaluation: " << sqlite3_errmsg(database) << endl;
		sqlite3_finalize(statement);
		return false;
	}
	if (sqlite3_changes(database) == 0) {
		sqlite3_finalize(statement);
		return false;
	}

	sqlite3_finalize(statement);
	return true;
}

bool DatabaseManager::deleteCourse(const string& courseNumber) {
	const char* sql = R"(DELETE FROM courses WHERE course_number = ?;)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to prepare course deletion: " << sqlite3_errmsg(database) << endl;
		return false;
	}

	sqlite3_bind_text(statement, 1, courseNumber.c_str(), -1, SQLITE_TRANSIENT);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE) {
		cerr << "Failed to delete course: " << sqlite3_errmsg(database) << endl;

		sqlite3_finalize(statement);
		return false;
	}
	if (sqlite3_changes(database) == 0) {
		sqlite3_finalize(statement);
		return false;
	}

	sqlite3_finalize(statement);
	return true;
}

bool DatabaseManager::deleteAssessment(const string& courseNumber, const string& assessmentName) {
	const char* sql = R"(DELETE FROM assessments
						WHERE course_id = (
							SELECT id FROM courses WHERE course_number = ?)
							AND assessment_name = ?;
					)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to prepare assessment deletion: " << sqlite3_errmsg(database) << endl;
		return false;
	}

	sqlite3_bind_text(statement, 1, courseNumber.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(statement, 2, assessmentName.c_str(), -1, SQLITE_TRANSIENT);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE) {
		cerr << "Failed to delete assessment: " << sqlite3_errmsg(database) << endl;
	
		sqlite3_finalize(statement);
		return false;
	}
	if (sqlite3_changes(database) == 0) {
		sqlite3_finalize(statement);
		return false;
	}

	sqlite3_finalize(statement);
	return true;
}

bool DatabaseManager::deleteEvaluation(const string& courseNumber, const string& assessmentName, const string& evaluationName) {
	const char* sql = R"(DELETE FROM evaluations
						WHERE assessment_id = (
							SELECT assessments.id
							FROM assessments
							INNER JOIN courses
								ON assessments.course_id = courses.id
							WHERE courses.course_number = ?
								AND assessments.assessment_name = ?)
						AND evaluation_name = ?;
					)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to prepare evaluation deletion: " << sqlite3_errmsg(database) << endl;
		return false;
	}

	sqlite3_bind_text(statement, 1, courseNumber.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(statement, 2, assessmentName.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(statement, 3, evaluationName.c_str(), -1, SQLITE_TRANSIENT);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE) {
		cerr << "Failed to delete evaluation: " << sqlite3_errmsg(database) << endl;

		sqlite3_finalize(statement);
		return false;
	}
	if (sqlite3_changes(database) == 0) {
		sqlite3_finalize(statement);
		return false;
	}

	sqlite3_finalize(statement);
	return true;
}

vector<Course> DatabaseManager::loadCourses() {
	vector<Course> courses;

	const char* sql = R"(SELECT course_number, passing_grade FROM courses;)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to load courses." << endl;
		return courses;
	}

	while (sqlite3_step(statement) == SQLITE_ROW) {
		const char* courseNumber = reinterpret_cast<const char*>(
			sqlite3_column_text(statement, 0));

		double passingGrade = sqlite3_column_double(statement, 1);

		Course course(courseNumber, passingGrade);

		vector<Assessment> assessments = loadAssessments(courseNumber);

		for (const Assessment& assessment : assessments) {
			course.addAssessment(assessment.getName(), assessment.getWeight());

			vector<Evaluation> evaluations = loadEvaluations(courseNumber, assessment.getName());

			for (const Evaluation& evaluation : evaluations)
				course.addEvaluation(assessment.getName(), evaluation.getName(), evaluation.getEarnedScore(), evaluation.getPossibleScore());
		}

		course.calculateGrade();

		courses.emplace_back(course);
	}

	sqlite3_finalize(statement);

	return courses;
}

vector<Assessment> DatabaseManager::loadAssessments(const string& courseNumber) {
	vector<Assessment> assessments;
	const char* sql = R"(
		SELECT assessments.assessment_name, assessments.weight
		FROM assessments
		INNER JOIN courses
			ON assessments.course_id = courses.id
		WHERE courses.course_number = ?
		ORDER BY assessments.id;
	)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to load assessments." << endl;
		return assessments;
	}

	sqlite3_bind_text(statement, 1, courseNumber.c_str(), -1, SQLITE_TRANSIENT);

	while (sqlite3_step(statement) == SQLITE_ROW) {
		const char* assessmentName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));

		double weight = sqlite3_column_double(statement, 1);

		assessments.emplace_back(assessmentName, weight);
	}

	sqlite3_finalize(statement);

	return assessments;
}

vector<Evaluation> DatabaseManager::loadEvaluations(const string& courseNumber, const string& assessmentName) {
	vector<Evaluation> evaluations;
	const char* sql = R"(
		SELECT evaluations.evaluation_name, evaluations.earned_score, evaluations.possible
		FROM evaluations
		INNER JOIN assessments
			ON evaluations.assessment_id = assessments.id
		INNER JOIN courses
			ON assessments.course_id = courses.id
		WHERE courses.course_number = ?
			AND assessments.assessment_name = ?
		ORDER BY CAST(
			SUBSTR(evaluations.evaluation_name, LENGTH('Assignment') + 1)
			AS INTEGER
		);
	)";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

	if (result != SQLITE_OK) {
		cerr << "Failed to load evaluations." <<
			sqlite3_errmsg(database) << endl;

		return evaluations;
	}

	sqlite3_bind_text(statement, 1, courseNumber.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(statement, 2, assessmentName.c_str(), -1, SQLITE_TRANSIENT);

	while (sqlite3_step(statement) == SQLITE_ROW) {
		const char* evaluationName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
		double earnedScore = sqlite3_column_double(statement, 1);
		double possibleScore = sqlite3_column_double(statement, 2);

		evaluations.emplace_back(evaluationName, earnedScore, possibleScore);
	}

	sqlite3_finalize(statement);

	return evaluations;
}

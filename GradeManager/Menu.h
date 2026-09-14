#ifndef MENU_H
#define MENU_H

#include "DatabaseManager.h"

void showMenu();
bool handleMenuChoice(DatabaseManager& database, int choice);
void viewCourses(DatabaseManager& database);
void addCourse(DatabaseManager& database);
void addAssessment(DatabaseManager& database);
void addEvaluation(DatabaseManager& database);
void deleteCourse(DatabaseManager& database);
void deleteAssessment(DatabaseManager& database);
void deleteEvaluation(DatabaseManager& database);

bool inputDouble(double& value);



#endif

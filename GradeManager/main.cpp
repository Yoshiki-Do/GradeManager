#include <iostream>
#include "DatabaseManager.h"
#include"Menu.h"

using namespace std;

int main() {

	DatabaseManager database("grades.db");

	database.createTables();

	int choice;

	while (true) {

		showMenu();
		cout << "Select: ";
		cin >> choice;

		if (!handleMenuChoice(database, choice))
			break;
	}
}
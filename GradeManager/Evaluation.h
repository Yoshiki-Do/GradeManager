#ifndef EVALUATION_H
#define EVALUATION

#include <string>

using namespace std;

class Evaluation {
private:
	string evaluationName;
	double earnedScore;
	double possibleScore;

public:
	Evaluation(const string& evaluationName, double earnedScore, double possibleScore);

	string getName()const;
	double getEarnedScore()const;
	double getPossibleScore()const;

	double getPercentage()const;
};

#endif
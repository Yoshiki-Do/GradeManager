#ifndef ASSESSMENT_H
#define ASSESSMENT_H

#include <string>
#include <vector>
#include "Evaluation.h"

using namespace std;

class Assessment {
private:
	string assessmentName;
	double weight;
	vector<Evaluation> evaluations;

public:
	Assessment(const string& assessmentName, double weight);

	string getName()const;
	double getWeight()const;
	const vector<Evaluation>& getEvaluations()const;

	void addEvaluation(const string& evaluationName, double earnedScore, double possibleScore);

	double getAveragePercentage()const;
	double getWeightedScore()const;
};


#endif
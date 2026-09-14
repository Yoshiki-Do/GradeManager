#include "Assessment.h"

Assessment::Assessment(const string& assessmentName, double weight)
	:assessmentName(assessmentName), weight(weight) {
}

string Assessment::getName() const {
	return assessmentName;
}

double Assessment::getWeight() const {
	return weight;
}

const vector<Evaluation>& Assessment::getEvaluations() const {
	return evaluations;
}

void Assessment::addEvaluation(const string& evaluationName, double earnedScore, double possibleScore) {
	evaluations.emplace_back(evaluationName, earnedScore, possibleScore);
}

double Assessment::getAveragePercentage() const {
	if (evaluations.empty())
		return 0.0;

	double totalPercentage = 0.0;
	for (const Evaluation& evaluation : evaluations) {
		totalPercentage += evaluation.getPercentage();
	}

	return totalPercentage / evaluations.size();
}

double Assessment::getWeightedScore() const {
	return getAveragePercentage() * weight;
}



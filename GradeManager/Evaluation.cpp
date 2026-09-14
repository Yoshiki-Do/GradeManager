#include "Evaluation.h"

Evaluation::Evaluation(const string& evaluationName, double earnedScore, double possibleScore)
	: evaluationName(evaluationName), earnedScore(earnedScore), possibleScore(possibleScore) {
}

string Evaluation::getName()const {
	return evaluationName;
}

double Evaluation::getEarnedScore() const {
	return earnedScore;
}

double Evaluation::getPossibleScore() const {
	return possibleScore;
}

double Evaluation::getPercentage()const {
	if (possibleScore == 0)
		return 0.0;

	return earnedScore / possibleScore;
}

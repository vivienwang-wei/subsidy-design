#include"definition.h"


void printErrorAndExit(const string &str, const exception &exc) {
	cout << "There is an error in " + str + " ! " << endl;
	cout << "Error information: " << exc.what() << endl;
	system("pause");
	exit(1);
}


double runTime(const clock_t &start)
{
	clock_t finish = clock();
	return (double)(finish - start) / CLOCKS_PER_SEC;
}


string getNowTime()
{
	ostringstream oss;
	oss << time(0);
	return oss.str();
}


bool lessThanReal(const double &lhs, const double &rhs, const double &threshold) {
	return lhs < rhs - threshold;
}


bool greaterThanReal(const double &lhs, const double &rhs, const double &threshold) {
	return lhs > rhs + threshold;
}


bool equalToReal(const double &lhs, const double &rhs, const double &threshold) {
	return !(lessThanReal(lhs, rhs, threshold) || greaterThanReal(lhs, rhs, threshold));
}


void print(const bool allowPrint, ostream &os, const string &str) {
	if (allowPrint) {
		os << str << endl;
	}
}


bool operator<=(const bitset<NMAX>& lhs, const bitset<NMAX>& rhs) {
	return (lhs | rhs) == rhs;
}


bool solveModel(IloCplex& cplex) {
	bool result = false;
	try {
		cplex.solve();
		result = cplex.getStatus() == IloAlgorithm::Optimal;
		cplex.out() << "solution status = " << cplex.getStatus() << endl;
		if (result) cplex.out() << "objective = " << cplex.getObjValue() << endl;
	}
	catch (const exception& exc) {
		printErrorAndExit("solveModel", exc);
	}
	return result;
}


bool isInteger(double num, double threshold) {
	return equalToReal(num, ceil(num), threshold) || equalToReal(num, floor(num), threshold);
}


bool isInteger(const vector<double>& nums, double threshold) {
	for (const auto num : nums)
		if (!isInteger(num, threshold))
			return false;

	return true;
}


// Set precision.
double setPrecision(const double num, const bool upDown, const int precision) {
	return upDown ? ceil(num * pow(10, precision)) / pow(10, precision) : floor(num * pow(10, precision)) / pow(10, precision);
}

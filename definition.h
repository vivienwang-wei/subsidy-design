#pragma once

#include"Header.h"

constexpr auto NFloat = 0;
constexpr auto RoundUpDown = true;

constexpr auto NMAX = 16;
constexpr auto Nseq = 5000;
constexpr auto Nty = 6;

constexpr auto Thousand = 1000;
constexpr auto TwentyFour = 24;
constexpr auto Ten = 10;
constexpr auto Seven = 7;
constexpr auto One = 1;
constexpr auto DECI = 0.1;
constexpr auto CENTI = 1e-2;
constexpr auto MILLI = 1e-3;
constexpr auto PPM = 1e-6;
constexpr auto InfinityPos = 1e10;
constexpr auto InfinityNeg = -InfinityPos;


constexpr auto ProfitLimit = 1e8;


typedef IloArray<IloNumVarArray> IloNumVarArray2;
typedef IloArray<IloNumVarArray2> IloNumVarArray3;
typedef IloArray<IloNumVarArray3> IloNumVarArray4;
typedef IloArray<IloIntVarArray> IloIntVarArray2;
typedef IloArray<IloIntVarArray2> IloIntVarArray3;
typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;

enum class OptimalityStatus { Infeasible, Feasible, Optimal };


void printErrorAndExit(const string &str, const exception &exc);
double runTime(const clock_t &start);
string getNowTime();
bool lessThanReal(const double &lhs, const double &rhs, const double &threshold);
bool greaterThanReal(const double &lhs, const double &rhs, const double &threshold);
bool equalToReal(const double &lhs, const double &rhs, const double &threshold);
void print(const bool allowPrint, ostream &os, const string &str);
bool operator<=(const bitset<NMAX>& lhs, const bitset<NMAX>& rhs);
bool solveModel(IloCplex& cplex);
bool isInteger(double num, double threshold);
bool isInteger(const vector<double>& nums, double threshold);
double setPrecision(const double num, const bool upDown, const int precision);


template<typename T>
void print1(ostream &os, const T &cont, const char character) {
	for (const auto &elem : cont) {
		os << elem << character;
	}
}


template<typename T>
void print2(ostream &os, const T &cont, const char character) {
	for (const auto &first : cont) {
		for (const auto &second : first) {
			os << second << character;
		}
		os << endl;
	}
}


template<typename T>
void read2(istream &ins, T &cont) {
	for (int i = 0; i < cont.size(); ++i) {
		for (int j = 0; j < cont[i].size(); ++j) {
			ins >> cont[i][j];
		}
	}
}


template<typename T>
void strToNum(const string &str, T &num) {
	stringstream ss;
	ss << str;
	ss >> num;
}


template<typename T>
string numToStr(const T &num) {
	string str;
	stringstream ss;
	ss << num;
	ss >> str;
	return str;
}


class thread_guard
{
private:
	thread &t;
public:
	explicit thread_guard(thread &myT) :t(myT) {}
	~thread_guard()
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	thread_guard(const thread_guard &) = delete;
	thread_guard& operator=(const thread_guard &) = delete;
};

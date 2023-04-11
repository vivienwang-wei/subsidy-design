#pragma once
#include"definition.h"
#include"Data.h"

class Solution {
public:
	double obj;
	vector<vector<int>> x;
	vector<double> s;
	vector<vector<double>> z;
	vector<vector<int>> noShipPerMethodPerSet;
	vector<vector<double>> ratioPerMethodPerSet;
	vector<vector<double>> ratioPerMethodCategory;
	vector<double> ratioPerMethod;
	vector<double> sPerCategory;
	double solutionTime;

	Solution() : obj(0), solutionTime(0) {}
	Solution(int scenario, const Parameter& parameter);
	void print() const;
};

void Objective(IloModel& model, IloExpr& expr, IloNumVarArray2 Z);
void AlphaCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X);
void BetaCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X);
void OneMethodCons(IloModel& model, IloExpr& expr, IloIntVarArray2 X);
void FeasibleCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X);
void ZCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X, IloNumVarArray2 Z, IloNumVarArray s);
void MethodCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X, IloNumVarArray s);

Solution getValue(int scenario, const Parameter& parameter, IloCplex cplex, IloIntVarArray2 X, IloNumVarArray2 Z, IloNumVarArray s);
void solve (int scenario, const Parameter& parameter);

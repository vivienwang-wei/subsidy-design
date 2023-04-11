#pragma once
#include"definition.h"

class Scenario {
public:
	vector<double> ratio;
};

class ScenarioSet {
public:
	int number;//the number of scenarios
	vector<Scenario> scenarios;
public:
	void set_scenarios(const string& input);
	void print() const;
};

class ShipType {
public:
	vector<double> cost;
	vector<int> feasible;
};

class TypeSet {
public:
	int category;
	vector<ShipType> typeSet;//size=Parameter.TypeNumber

public:
	void set_cost(const string& input);
	void set_feasible(const string& input);
	void print() const;
};

class Parameter {
public:
	int TypeNumber;
	int MethodNumber;
	double alpha;
	double beta;
	vector<int> shipNumber;
	vector<vector<vector<double>>> cost;
	vector<vector<vector<int>>> methodFeasible;
	vector<vector<int>> noPerSet;
	vector<int> totalNo;
	vector<vector<int>> shipPerCategory;
public:
	void set_noPerSet(const ScenarioSet& scenario);
	void set_shipNumber();
	void set_cost(const TypeSet& set);
	void set_methodFeasible(const TypeSet& set);
	void print() const;
};

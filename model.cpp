#include"model.h"

Solution::Solution(int scenario, const Parameter& parameter) {
	try {
		obj = 0;
		solutionTime = 0;
		x.resize(parameter.cost[scenario].size());
		z.resize(parameter.cost[scenario].size());
		for (int v = 0; v < x.size(); ++v) {
			x[v].resize(parameter.cost[scenario][v].size());
			z[v].resize(parameter.cost[scenario][v].size());
		}
		s= vector<double>(parameter.MethodNumber, 0);
		ratioPerMethod = vector<double>(parameter.MethodNumber, 0);
		noShipPerMethodPerSet = vector<vector<int>>(parameter.TypeNumber, vector<int>(parameter.MethodNumber, 0));
		ratioPerMethodPerSet = vector<vector<double>>(parameter.TypeNumber, vector<double>(parameter.MethodNumber, 0));
		sPerCategory.resize(4);
		ratioPerMethodCategory= vector<vector<double>>(4, vector<double>(parameter.MethodNumber, 0));
		sPerCategory=vector<double>(4, 0);
	}
	catch (const exception& exc) {
		printErrorAndExit("Solution::Solution", exc);
	}
}

void Solution::print() const {
	try {
		cout << "objective= " << obj << endl;
		cout << "the subsidy for each method = " << "\t";
		for (int i = 0; i < s.size(); ++i) cout << s[i] << "\t";
		cout << endl;
		cout << "the number of ships in each type using each enery method =" << endl;
		for (int t = 0; t < noShipPerMethodPerSet.size(); ++t) {
			cout << "type " << t << " =" << "\t";
			for (int i = 0; i < noShipPerMethodPerSet[t].size(); ++i) cout<< noShipPerMethodPerSet[t][i] << "\t";
			cout << endl;
		}
		cout << "the ratio of ships in each type using each enery method =" << endl;
		for (int t = 0; t < ratioPerMethodPerSet.size(); ++t) {
			cout << "type " << t << " =" << "\t";
			for (int i = 0; i < ratioPerMethodPerSet[t].size(); ++i) cout << ratioPerMethodPerSet[t][i] << "\t";
			cout << endl;
		}
		cout << "the ratio of ships in each category using each enery method =" << endl;
		for (int t = 0; t < ratioPerMethodCategory.size(); ++t) {
			cout << "category " << t << " =" << "\t";
			for (int i = 0; i < ratioPerMethodCategory[t].size(); ++i) cout << ratioPerMethodCategory[t][i] << "\t";
			cout << endl;
		}
		cout << "the ratio of ships using each enery method =" << endl;
		for (int i = 0; i < ratioPerMethod.size(); ++i) cout << ratioPerMethod[i] << "\t";
		cout << endl;
		cout << "the subsidy for each category =" << endl;
		for (int c = 0; c < sPerCategory.size(); ++c) cout << sPerCategory[c] << "\t";
		cout << endl;
		cout << "running time =" << solutionTime<< endl;
	}
	catch (const exception& exc) {
		printErrorAndExit("Solution::print()", exc);
	}
}

void Objective(IloModel& model, IloExpr& expr, IloNumVarArray2 Z) {
	try {
		auto env = model.getEnv();
		IloObjective objective = IloMinimize(env);
		expr.clear();
		for (int v = 0; v < Z.getSize(); ++v) {
			for (int i = 0; i < Z[v].getSize(); ++i) {
				expr += Z[v][i];
			}
		}
		objective.setExpr(expr);
		model.add(objective);
	}
	catch (const exception& exc) {
		printErrorAndExit("Objective", exc);
	}
}

void AlphaCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X) {
	try {
		expr.clear();
		for (int v = 0; v < X.getSize(); ++v) {
			expr += X[v][1];
		}
		model.add(expr >= parameter.alpha* parameter.shipNumber[scenario]);
	}
	catch (const exception& exc) {
		printErrorAndExit("AlphaCons", exc);
	}
}

void BetaCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X) {
	try {
		expr.clear();
		for (int v = 0; v < X.getSize(); ++v) {
			expr += X[v][2];
		}
		model.add(expr >= parameter.beta * parameter.shipNumber[scenario]);
	}
	catch (const exception& exc) {
		printErrorAndExit("BetaCons", exc);
	}
}

void OneMethodCons(IloModel& model, IloExpr& expr, IloIntVarArray2 X) {
	try {
		for (int v = 0; v < X.getSize(); ++v) {
			expr.clear();
			for (int i = 0; i < X[v].getSize(); ++i) {
				expr += X[v][i];
			}
			model.add(expr == 1);
		}
	}
	catch (const exception& exc) {
		printErrorAndExit("OneMethodCons", exc);
	}
}

void FeasibleCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X) {
	try {
		for (int v = 0; v < X.getSize(); ++v) {
			for (int i = 0; i < X[v].getSize(); ++i) {
				expr.clear();
				expr += X[v][i]; 
				model.add(expr <= parameter.methodFeasible[scenario][v][i]);
			}
		}
	}
	catch (const exception& exc) {
		printErrorAndExit("FeasibleCons", exc);
	}
}

void ZCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X, IloNumVarArray2 Z, IloNumVarArray s) {
	try {
		double max = 0;
		for (int v = 0; v < parameter.cost[scenario].size(); ++v) {
			for (int i = 0; i < parameter.cost[scenario][v].size(); ++i) {
				if (max < parameter.cost[scenario][v][i]) max = parameter.cost[scenario][v][i];
			}
		}
		for (int v = 0; v < X.getSize(); ++v) {
			for (int i = 0; i < X[v].getSize(); ++i) {
				expr.clear();
				expr += Z[v][i] - s[i] - max * (X[v][i] - 1);
				model.add(expr >= 0);
			}
		}
	}
	catch (const exception& exc) {
		printErrorAndExit("ZCons", exc);
	}
}

void MethodCons(int scenario, const Parameter& parameter, IloModel& model, IloExpr& expr, IloIntVarArray2 X, IloNumVarArray s) {
	try {
		for (int v = 0; v < X.getSize(); ++v) {
			double max = 0;
			for (int i = 0; i < X[v].getSize(); ++i) {
				if (max < parameter.cost[scenario][v][i]) max = parameter.cost[scenario][v][i];
			}
			for (int i = 0; i < X[v].getSize(); ++i) {
				for (int j = 0; j < X[v].getSize(); ++j) {
					if (j != i) {
						expr.clear();
						expr += parameter.cost[scenario][v][i] - s[i]- (parameter.cost[scenario][v][j] - s[j]) - max * (1 - X[v][i]+1-parameter.methodFeasible[scenario][v][j]);
						model.add(expr <= 0);
					}
				}
			}
		}
	}
	catch (const exception& exc) {
		printErrorAndExit("MethodCons", exc);
	}
}

Solution getValue(int scenario, const Parameter& parameter, IloCplex cplex, IloIntVarArray2 X, IloNumVarArray2 Z, IloNumVarArray s) {
	Solution result(scenario, parameter);
	try {
		result.obj= cplex.getObjValue();
		for (int v = 0; v < X.getSize(); ++v) {
			for (int i = 0; i < X[v].getSize(); ++i) {
				result.x[v][i]= cplex.getValue(X[v][i]);
				result.z[v][i] = cplex.getValue(Z[v][i]);
			}
		}
		for (int m = 0; m < s.getSize(); ++m) {
			result.s[m]= cplex.getValue(s[m]);
		}
		int pre = 0;
		int pos = 0;
		for (int type = 0; type < parameter.TypeNumber; ++type) {
			pos += parameter.noPerSet[scenario][type];
			for (int v = pre; v < pos; ++v) {
				for (int i = 0; i < result.x[v].size(); ++i) {
					if (equalToReal(result.x[v][i], 1, PPM)) {
						result.noShipPerMethodPerSet[type][i] += 1;
						result.ratioPerMethodPerSet[type][i] += 1.0 / double(parameter.noPerSet[scenario][type]);
						break;
					}
				}
			}
			pre = pos;
		for (int t = 0; t < result.noShipPerMethodPerSet.size(); ++t) {
			for (int i = 0; i < parameter.MethodNumber; ++i) {
				result.ratioPerMethodCategory[t / 4][i] += double(result.noShipPerMethodPerSet[t][i]) / double(parameter.shipPerCategory[scenario][t / 4]);
			}
		}
		for (int v = 0; v < result.x.size(); ++v) {
			for (int i = 0; i < result.x[v].size(); ++i) {
				if (equalToReal(result.x[v][i], 1, PPM)) {
					result.ratioPerMethod[i] += 1.0 / double(parameter.shipNumber[scenario]);
					break;
				}
			}
		}
		for (int c = 0; c < result.ratioPerMethodCategory.size(); ++c) {
			for (int i = 0; i < result.ratioPerMethodCategory[c].size(); ++i) {
				result.sPerCategory[c]+=result.ratioPerMethodCategory[c][i] * parameter.shipPerCategory[scenario][c] * result.s[i];
			}
		}

	}
	catch (const exception& exc) {
		printErrorAndExit("getValue", exc);
	}
	return result;
}

void solve(int scenario, const Parameter& parameter) {
	try {
		Solution result(scenario, parameter);
		clock_t start = clock();
		IloEnv env;
		IloModel model(env);
		IloExpr expr(env);
		IloIntVarArray2 X(env, parameter.cost[scenario].size());
		IloNumVarArray2 Z(env, parameter.cost[scenario].size());
		for (int v = 0; v < X.getSize(); ++v) {
			X[v]= IloIntVarArray (env, parameter.cost[scenario][v].size(), 0, 1);
			Z[v]= IloNumVarArray(env, parameter.cost[scenario][v].size(), 0, IloInfinity);
		}
		IloNumVarArray s(env, parameter.MethodNumber, 0, IloInfinity);
		Objective(model, expr, Z);
		AlphaCons(scenario, parameter, model, expr, X);
		BetaCons(scenario, parameter, model, expr, X);
		OneMethodCons(model, expr, X);
		FeasibleCons(scenario, parameter, model, expr, X);
		ZCons(scenario, parameter, model, expr, X, Z, s);
		MethodCons(scenario, parameter, model, expr, X, s);

		IloCplex cplex(model);
		cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1e-6);
		cplex.solve();
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			cout << "最优解找到了" << endl;
			result= getValue(scenario, parameter, cplex, X, Z, s);
			result.solutionTime = runTime(start);
			result.print();
		}
		else {
			cout << "最优解未找到" << endl;
		}
	}
	catch (const exception& exc) {
		printErrorAndExit("solve", exc);
	}
}

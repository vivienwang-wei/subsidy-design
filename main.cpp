#include"Data.h"
#include"definition.h"
#include"model.h"

int main(int argc, char** argv) {
	try {
		ScenarioSet SS;
		SS.set_scenarios("imput\\scenario.txt");
		SS.print();

		TypeSet TS;
		TS.set_cost("imput\\typeCost.txt");
		TS.set_feasible("imput\\typeFeasible.txt");
		TS.print();

		Parameter parameter;
		parameter.TypeNumber = TS.category;
		parameter.MethodNumber = 3;
		parameter.totalNo = {6612,2628,1608,1152};
		parameter.alpha = 0.05;
		parameter.beta = 0.05;
		parameter.set_noPerSet(SS);
		parameter.set_shipNumber();
		parameter.set_cost(TS);
		parameter.set_methodFeasible(TS);
		parameter.print();
		
		solve(6, parameter);
	}
	catch (const exception& exc) {
		printErrorAndExit("main", exc);
	}
	return 0;
}

#include"Data.h"


void ScenarioSet::set_scenarios(const string& input) {
    try {
        ifstream file(input);
        if (!file) throw exception();

        file >> number;
        scenarios.resize(number);

        string temp;
        getline(file, temp);
        getline(file, temp);

        for (int i = 0; i < number; ++i) {
            file >> temp;
            scenarios[i].ratio.resize(4);
            for (int j = 0; j < 4; ++j) {
                file >> scenarios[i].ratio[j];
            }
        }
        file.close();
    }
    catch (const exception& exc) {
        printErrorAndExit("ScenarioSet::set_scenarios", exc);
    }
}

void ScenarioSet::print()const {
    for (int i = 0; i < scenarios.size(); ++i) {
        cout << "scenario " << i << " = " << "\t";
        for (int j = 0; j < scenarios[i].ratio.size(); ++j) {
            cout << scenarios[i].ratio[j] << "\t";
        }
        cout << endl;
    }
}

void TypeSet::set_cost(const string& input) {
    ifstream file(input);
    if (!file) throw exception();

    file >> category;
    typeSet.resize(category);

    string temp;
    getline(file, temp);
    getline(file, temp);

    for (int i = 0; i < category; ++i) {
        typeSet[i].cost.resize(3);
        file >> temp;
        for (int j = 0; j < 3; ++j) file >> typeSet[i].cost[j];
    }
    file.close();
}

void TypeSet::set_feasible(const string& input) {
    ifstream file(input);
    if (!file) throw exception();

    string temp;
    getline(file, temp);

    for (int i = 0; i < category; ++i) {
        typeSet[i].feasible.resize(3);
        file >> temp;
        for (int j = 0; j < 3; ++j) file >> typeSet[i].feasible[j];
    }
    file.close();
}

void TypeSet::print() const {
    cout << "energy cost per ship call (USD): " << endl;
    for (int i = 0; i < typeSet.size(); ++i) {
        for (int j = 0; j < typeSet[i].cost.size(); ++j) cout << typeSet[i].cost[j] << "\t";
        cout << endl;
    }
    cout << "whether the ship in this category could use corresponding devices: " << endl;
    for (int i = 0; i < typeSet.size(); ++i) {
        for (int j = 0; j < typeSet[i].feasible.size(); ++j) cout << typeSet[i].feasible[j] << "\t";
        cout << endl;
    }
}

void Parameter::set_noPerSet(const ScenarioSet& scenario) {
    noPerSet.resize(scenario.number);
    for (int i = 0; i < scenario.number; ++i) {
        noPerSet[i].resize(TypeNumber);
        for (int j = 0; j < TypeNumber; ++j) {
            int pos1 = j / 4;
            int pos2 = j % 4;
            double temp = totalNo[pos1] * scenario.scenarios[i].ratio[pos2];
            noPerSet[i][j] = round(temp);
        }
    }
}

void Parameter::set_shipNumber() {
    shipNumber.resize(noPerSet.size());
    shipPerCategory.resize(noPerSet.size());
    for (int i = 0; i < noPerSet.size(); ++i) {
        int num = 0;
        vector<int> total(4, 0);
        for (int j = 0; j < noPerSet[i].size(); ++j) {
            total[j / 4]+= noPerSet[i][j];
            num += noPerSet[i][j];
        }
        shipNumber[i] = num;
        shipPerCategory[i] = total;
    }
}

void Parameter::set_cost(const TypeSet& set) {
    cost.resize(noPerSet.size());
    for (int i = 0; i < noPerSet.size(); ++i) {  
        for (int j = 0; j < noPerSet[i].size(); ++j) {
            for (int v = 0; v < noPerSet[i][j]; ++v) {
                vector<double> tempCost(MethodNumber);
                for (int k = 0; k < MethodNumber; ++k) {
                    tempCost[k] = set.typeSet[j].cost[k];
                }
                cost[i].push_back(tempCost);
            }
        }
    }
}

void Parameter::set_methodFeasible(const TypeSet& set) {
    methodFeasible.resize(noPerSet.size());
    for (int i = 0; i < noPerSet.size(); ++i) {
        for (int j = 0; j < noPerSet[i].size(); ++j) {
            for (int v = 0; v < noPerSet[i][j]; ++v) {
                vector<int> tempMethodFeasible(MethodNumber);
                for (int k = 0; k < MethodNumber; ++k) {
                    tempMethodFeasible[k] = set.typeSet[j].feasible[k];
                }
                methodFeasible[i].push_back(tempMethodFeasible);
            }
        
        }
    }
}

void Parameter::print()const{
    cout << "The number of categories = " << TypeNumber << endl;
    cout << "The number of energy supply methods = " << MethodNumber << endl;
    cout << "alpha = " << alpha << endl;
    cout << "beta = " << beta << endl;
    for (int i = 0; i < noPerSet.size(); ++i) {
        cout << "------------- Parameters for scenario " << i << " -------------" << endl;
        cout << "the number of ship calls under scenario " << i << " =" << shipNumber[i]<<endl;
        cout << "the number of ship calls for each type under scenario " << i << " =" << "\t";
        for (int j = 0; j < noPerSet[i].size(); ++j) {
            cout << noPerSet[i][j] << "\t";
        }
        cout << endl;
        cout << "the number of ship calls for each category under scenario " << i << " =" << "\t";
        for (int j = 0; j < shipPerCategory[i].size(); ++j) {
            cout << shipPerCategory[i][j] << "\t";
        }
        cout << endl;
    }
}

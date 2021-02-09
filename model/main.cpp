#include <iostream>
#include <string>
#include "calc.h"

using namespace std;

int main(int argc, char* argv[]) {
    Environment environment;

	cout << "Start of " << argv[0] << endl;
	vector<Action> action;
	string s;

	// Set the sequence of actions if using command line arguments
	for (int i = 1; i < argc; i++) {
	    s = string(argv[i]);
        action.emplace_back(static_cast<Action>(stoi(s)));
	}
	// Set the start (and the only) action if using file input
	if (argc == 1) {
	    action.push_back(environment.action);
	}

	for (Action a : action) {
	    environment.action = a;
        environment.setSystem(); // not a good idea :(
        environment.execute();
	}

	cout << argv[0] << " ended successfully" << endl;
	return 0;
}
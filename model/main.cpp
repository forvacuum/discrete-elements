#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include "calc.h"

using namespace std;

int main(int argc, char* argv[]) {
    ofstream log("log.txt");

    time_t t = time(nullptr);
    tm now{};
    localtime_s(&now, &t);
    log << put_time(&now, "%D %X") << " Start of " << argv[0] << endl;

    Environment environment;
	vector<Action> action;
	string s;

    log << environment.getDetails() << endl;
    log << "Actions requested: ";
	// Set the sequence of actions if using command line arguments
	for (int i = 1; i < argc; i++) {
	    log << argv[i] << " ";
	    s = string(argv[i]);
        action.emplace_back(static_cast<Action>(stoi(s)));
	}
	// Set the start (and the only) action if using file input
	if (argc == 1) {
	    log << environment.action;
	    action.push_back(environment.action);
	}
	log << endl;

	for (Action a : action) {
	    log << "Current action: " << a << endl;
	    environment.action = a;
        environment.setSystem(); // not a good idea :(
        try {
            environment.execute();
        } catch (ParticleOutOfBorderException& e) {
            log << "The particle with coordinates " << e.what()->position
                << " has crossed the border. Program execution is stopped" << endl;
            break;
        }
	}

	t = time(nullptr);
    localtime_s(&now, &t);
	log << put_time(&now, "%D %X") << ' ' << argv[0] << " ended successfully" << endl;
	return 0;
}
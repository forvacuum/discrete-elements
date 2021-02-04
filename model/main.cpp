#include <iostream>

#include "calc.h"

using namespace std;

int main(int argc, char* argv[]) {
    Environment environment;

	cout << "Start of " << argv[0] << endl;
    exportDetails(R"(C:\Users\Veronika\Documents\visualisation\info.txt)", environment.border, environment.system);
	environment.execute();

//    auto system = importParticles(R"(C:\Users\Veronika\discrete-elements\auxiliary\converted.txt)", constantsFile);

	cout << argv[0] << " ended successfully" << endl;
	return 0;
}
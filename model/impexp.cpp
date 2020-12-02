#include "impexp.h"
#include "parameter.h"
#include "calc.h"

using namespace std;

vector<Particle> importParticles(const string &sourceFile, const string &constantsFile) {
	ifstream fin(sourceFile);
    ifstream fin_const(constantsFile);
	Particle p;
	vector<Particle> system;
	Vector relativePosition;
	int peekValue;
	double density;
	double minRadius;
	double maxRadius = 0;

	while ((peekValue = fin.peek()) != EOF) {
		if (peekValue == ' ' || peekValue == '\n') {
			fin.get();
		}
		else {
		fin >> p.radius >> density;
		fin >> p.position >> p.velocity;
        minRadius = p.radius;
        p.mass = density * pi * p.radius * p.radius;
		if (p.radius > maxRadius) {
			maxRadius = p.radius;
		}
        if (p.radius < minRadius) {
            minRadius = p.radius;
        }

		system.push_back(p);
		}
	}
	fin.close();

	fin_const >> Particle::stiffnessRepulsive;
	fin_const >> Particle::stiffnessAttractive;
	fin_const >> Particle::stiffnessShear;
	fin_const >> Particle::particleFriction;
	fin_const >> Particle::wallFriction;
	fin_const >> Particle::floorFriction;
	fin_const >> Particle::criticalDistance;

	Particle::minRadius = minRadius;
	Particle::maxRadius = maxRadius;

	return system;
}

void exportParticles(const std::string& filename, const vector<Particle>& system) {
    ofstream fout(filename);
    auto it = system.begin();
    while (it != system.end()) {
        fout << it->radius << " " << it->mass << " ";
        fout << it->position << it->velocity << endl;
        it++;
    }

    fout.close();
}

void exportDetails(const std::string& filename, const double border[4], const vector<Particle>& system) {
	ofstream fout(filename);
	auto it = system.begin();

	for (int i = 0; i < 4; i++) {
		fout << border[i] << " ";
	}
	while (it != system.end()) {
		fout << it->radius << " ";
		it++;
	}

	fout.close();
}

void appendSystemPosition(std::ofstream& fout, const std::vector<Particle>& system) {
	auto it = system.begin();
	while (it != system.end()) {
		fout << it->position << " ";
		it++;
	}
	fout << endl;
}

double appendSystemEnergy(std::ofstream& fout, const std::vector<Particle>& system, Grid& grid, const double border[4]) {
	auto it = system.begin();
	double systemEnergy = 0;
	double tmp;

	while (it != system.end()) {
		tmp = calculateTotalEnergy(*it, border, grid);
		systemEnergy += tmp;
		fout << tmp << " ";
		it++;
	}
	fout << endl;

	return systemEnergy;
}

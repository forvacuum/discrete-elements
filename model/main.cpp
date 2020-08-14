#include <iostream>
#include <vector>
#include <iterator>
#include <exception>

#include "particle.h"
#include "impexp.h"
#include "force.h"
#include "physics.h"

using namespace std;

/* TODO:
 *	calculateTotalEnergy(...) uses the same code as in the applyRepulsion(...) to calculate delta
 *	generateParticles(...): solve the problem of particles overlapping at their initial positions */

int main(int argc, char* argv[]) {
	string infoFilename;
	string particlesFilename;
	string generatorInfoFilename;
	string outputFilename = "C:\\Users\\Veronika\\workspace\\dem\\visualisation\\output.txt";
	string outputEnergyFilename = "C:\\Users\\Veronika\\workspace\\dem\\visualisation\\energy.txt";

	double border[4];
	double maxTime;
	double timestep;
	double stiffness;
	bool generatorEnabled;

	/* Parameters initialization */

	if (argc < 3) {
		infoFilename = "C:\\Users\\Veronika\\workspace\\dem\\model\\model\\info.txt";
		generatorInfoFilename = "C:\\Users\\Veronika\\workspace\\dem\\model\\model\\generatorinfo.txt";
		particlesFilename = "C:\\Users\\Veronika\\workspace\\dem\\model\\model\\particles.txt";
	}
	else {
		infoFilename = argv[1];
		generatorInfoFilename = argv[2];
		particlesFilename = argv[2];
		/* Note: actual choice is user-made and depends on a 'generatorEnabled' flag 
				 stored in the info file. */
	}
	
	ifstream fin(infoFilename);
	for (int i = 0; i < 4; i++) {
		fin >> border[i];
	}
	fin >> maxTime >> timestep >> stiffness >> generatorEnabled;
	fin.close();

	if (generatorEnabled) {
		particlesFilename = "C:\\Users\\Veronika\\workspace\\dem\\model\\model\\particles.txt";
		generateParticles(generatorInfoFilename, particlesFilename, border);
	}

	/* Model computation algorithm */

	auto system = importParticles(particlesFilename, stiffness);
	vector<Particle>::iterator it;
	double currentTime = 0;

	ofstream fout(outputFilename);
	ofstream fout_kinetic("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\kinetic.txt");
	ofstream fout_potential("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\potential.txt");
	ofstream fout_elastic_p("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\elastic_p.txt");
	ofstream fout_elastic_w("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\elastic_w.txt");

	while (currentTime < maxTime) {
		fout << currentTime << " ";
		it = system.begin();

		while (it != system.end()) {
			fout << it->position;
			fout_kinetic << kineticEnergy(*it) << " ";
			fout_potential << potentialEnergy(*it, border) << " ";
			fout_elastic_p << elasticParticleEnergy(*it, system) << " ";
			fout_elastic_w << elasticWallEnergy(*it, border) << " ";

			Verlet(*it, system, timestep, border);
			it++;
		}

		fout << endl;
		fout_kinetic << endl;
		fout_potential << endl;
		fout_elastic_p << endl;
		fout_elastic_w << endl;

		currentTime += timestep;
	}

	fout.close();
	fout_kinetic.close();
	fout_potential.close();
	fout_elastic_p.close();
	fout_elastic_w.close();

	exportDetails("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\info.txt", border, system);

	cout << "Program model.exe ended succesfully" << endl;

	return 0;
}


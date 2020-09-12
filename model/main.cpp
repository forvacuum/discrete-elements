#include <iostream>
#include <vector>
#include <iterator>
#include <exception>

#include "particle.h"
#include "impexp.h"
#include "force.h"
#include "physics.h"
#include "grid.h"

using namespace std;

int main(int argc, char* argv[]) {

	string infoFilename = "C:\\Users\\Veronika\\Documents\\GitHub\\discrete-elements\\model\\info.txt";
	string particlesFilename = "C:\\Users\\Veronika\\Documents\\GitHub\\discrete-elements\\particles.txt";
	string generatorInfoFilename = "C:\\Users\\Veronika\\Documents\\GitHub\\discrete-elements\\model\\generatorinfo.txt";
	string outputFilename = "C:\\Users\\Veronika\\workspace\\dem\\visualisation\\output.txt";
	string outputEnergyFilename = "C:\\Users\\Veronika\\workspace\\dem\\visualisation\\energy.txt";

	double border[4];
	double maxTime;
	double timestep;
	double stiffness;
	bool generatorEnabled;

	ofstream log(stdout);

	log << "Program started" << endl;

	/* Parameters initialization */

	log << "Initializing parameters" << endl;

	ifstream fin(infoFilename);
	for (int i = 0; i < 4; i++) {
		fin >> border[i];
	}

	log << "System borders: ";
	for (int i = 0; i < 4; i++) {
		log << border[i] << " ";
	}
	log << endl;

	fin >> maxTime >> timestep >> stiffness >> generatorEnabled;

	log << "Maximum time: " << maxTime << endl;
	log << "Time step: " << timestep << endl;
	log << "Stiffness: " << stiffness << endl;
	log << "Generator enabled: " << generatorEnabled << endl;
	fin.close();

	if (generatorEnabled) {
		particlesFilename = "C:\\Users\\Veronika\\Documents\\GitHub\\discrete-elements\\particles.txt";
		generateParticlesTriangle(generatorInfoFilename, particlesFilename, border);
		log << "Particles generation is finished" << endl;
	}

	/* Model computation algorithm */
	log << "Starting algorithm" << endl;

	auto system = importParticles(particlesFilename, stiffness, border);
	GridCell::defaultSize = Particle::maxRadius * 2;

	log << "Particles are imported" << endl;

	auto grid = GridCell::setGrid(border);
	Particle::setGridCellPositions(system, grid);
	GridCell::setCellsContents(grid, system);
	refreshDeltaWall(grid, border);

	vector<Particle>::iterator it;
	double currentTime = 0;

	ofstream fout(outputFilename);
	ofstream fout_e(outputEnergyFilename);
  
	while (currentTime < maxTime) {
		fout << currentTime << " ";
		appendSystemPosition(fout, system);
		appendSystemEnergy(fout_e, system, grid, border);
		calculateNextIteration(system, grid, timestep, border);
		currentTime += timestep;
	}

	fout.close();

	exportDetails("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\info.txt", border, system);
	log << "Details are exported" << endl;

	log << "Program model.exe ended succesfully" << endl;

	return 0;
}
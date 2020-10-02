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
	double timestep;
	double stiffnessRepulsive;
	double stiffnessAttractive;
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

	fin >> timestep >> stiffnessRepulsive >> stiffnessAttractive >> generatorEnabled;

	log << "Time step: " << timestep << endl;
	log << "Stiffness on repulsion: " << stiffnessRepulsive << endl;
	log << "Stiffness on cohesion: " << stiffnessAttractive << endl;
	log << "Generator enabled: " << generatorEnabled << endl;
	fin.close();

	if (generatorEnabled) {
		particlesFilename = "C:\\Users\\Veronika\\Documents\\GitHub\\discrete-elements\\particles.txt";
		generateParticlesTriangle(generatorInfoFilename, particlesFilename, border);
		log << "Particles generation is finished" << endl;
	}

	/* Model computation algorithm */
	log << "Starting algorithm" << endl;

	auto system = importParticles(particlesFilename, stiffnessRepulsive, stiffnessAttractive, border);
	GridCell::defaultSize = Particle::maxRadius * 2;

	log << "Particles are imported" << endl;

	auto grid = GridCell::setGrid(border);
	Particle::setGridCellPositions(system, grid);
	GridCell::setCellsContents(grid, system);
	Particle::refreshDeltaWall(grid, border);

	vector<Particle>::iterator it;
	double preparationTime = 0;
	double workTime = 0;
	double totalTime = 0;
	double systemEnergy = 0;
	double energyDiff = 0;
	double eps = 1e-6;

	ofstream fout(outputFilename);
	ofstream fout_e(outputEnergyFilename);

	do {
		fout << preparationTime << " ";
		appendSystemPosition(fout, system);
		energyDiff = systemEnergy;
		systemEnergy = appendSystemEnergy(fout_e, system, grid, border);
		energyDiff -= systemEnergy;
		calculateNextIteration(system, grid, timestep, border);
		preparationTime += timestep;

	} while (abs(energyDiff) >= eps || preparationTime < 1);

	log << "Packing is ready" << endl;
	Particle::isPacked = true;
	//Particle::isWallEnabled[1] = false;
	border[1] *= 2;
	totalTime += preparationTime;

	do {
		fout << totalTime << " ";
		appendSystemPosition(fout, system);
		energyDiff = systemEnergy;
		systemEnergy = appendSystemEnergy(fout_e, system, grid, border);
		energyDiff -= systemEnergy;
		calculateNextIteration(system, grid, timestep, border);
		workTime += timestep;
		totalTime += timestep;

	} while (abs(energyDiff) >= eps || workTime < 1);

	fout.close();

	exportDetails("C:\\Users\\Veronika\\workspace\\dem\\visualisation\\info.txt", border, system);
	log << "Details are exported" << endl;

	log << "Program model.exe ended succesfully" << endl;

	return 0;
}
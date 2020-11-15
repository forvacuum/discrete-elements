#include <iostream>
#include <vector>
#include <iterator>

#include "particle.h"
#include "impexp.h"

using namespace std;

int main(int argc, char* argv[]) {
    string generatorInfoFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\generatorinfo.txt)";
    string constantsFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\const.txt)";
	string particlesFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\particles.txt)";
	string outputFile = R"(C:\Users\Veronika\Documents\visualisation\output.txt)";
	string outputEnergyFile = R"(C:\Users\Veronika\Documents\visualisation\energy.txt)";

	double border[4];
	double timeStep;
	bool generatorEnabled;
	bool prePacked;
    bool packOnly;

	ofstream log(stdout);

	log << "Program started" << endl;
	log << "Initializing parameters" << endl;
	initialize(border, timeStep, generatorEnabled, prePacked, packOnly);

	if (!packOnly && prePacked) {
        particlesFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\packed.txt)";
        generatorEnabled = false;
        Particle::isPacked = true;
    }
	if (generatorEnabled) {
		generateParticlesTriangle(generatorInfoFile, particlesFile, border);
		log << "Particles generation is finished" << endl;
	}

	log << "Starting algorithm" << endl;
	auto system = importParticles(particlesFile, constantsFile);
	GridCell::defaultSize = Particle::maxRadius * 2;
	log << "Particles are imported" << endl;

	auto grid = GridCell::setGrid(border);
	Particle::setGridCellPositions(system, grid);
	GridCell::setCellsContents(grid, system);
	Particle::refreshDeltaWall(grid, border);

	vector<Particle>::iterator it;
    double time = 0;
	exportDetails(R"(C:\Users\Veronika\Documents\visualisation\info.txt)", border, system);
	log << "Details are exported" << endl;

	ofstream fout(outputFile);
	ofstream fout_e(outputEnergyFile);

	if(!Particle::isPacked) {
        time = pack(fout, fout_e, system, grid, timeStep, border);
        log << "Packing is ready" << endl;
        Particle::isPacked = true;
        exportParticles(R"(C:\Users\Veronika\discrete-elements\auxiliary\packed.txt)", system);
    }
    fout << time << " ";
    appendSystemPosition(fout, system);
	if(!packOnly) {
        //Particle::isWallEnabled[1] = false;
        border[1] *= 2;
        setNeighbours(system, grid);
        execute(fout, fout_e, system, grid, timeStep, time, border);
        fout << time << " ";
        appendSystemPosition(fout, system);
	}
	fout.close();
    fout_e.close();
	log << "Program model.exe ended successfully" << endl;
	return 0;
}
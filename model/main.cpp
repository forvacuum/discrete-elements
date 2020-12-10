#include <iostream>
#include <vector>
#include <iterator>

#include "particle.h"
#include "impexp.h"
#include "calc.h"
#include "grid.h"

using namespace std;

int main(int argc, char* argv[]) {
    string generatorInfoFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\generatorinfo.txt)";
    string constantsFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\const.txt)";
	string particlesFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\particles.txt)";
	string outputFile = R"(C:\Users\Veronika\Documents\visualisation\output.txt)";
	string outputEnergyFile = R"(C:\Users\Veronika\Documents\visualisation\energy.txt)";

	double border[4];
	double* workspace;
	double timeStep;
	bool generatorEnabled;
	bool prePacked;
    bool packOnly;
    Particle* wentOut = nullptr;

	ofstream log(stdout);
	log << "Start of " << argv[0] << endl;
	initialize(border, timeStep, generatorEnabled, prePacked, packOnly);
    workspace = setWorkspace(border);
    log << "Parameters are initialized" << endl;

	if (!packOnly && prePacked) {
        particlesFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\packed.txt)";
        generatorEnabled = false;
        Particle::isPacked = true;
    }
	if (generatorEnabled) {
		generateParticlesTriangle(generatorInfoFile, particlesFile, border);
		log << "Particles generation is finished" << endl;
	}

    log << "Algorithm is started" << endl;
    auto system = importParticles(particlesFile, constantsFile);

	log << "Particles are imported" << endl;

//    auto system = importParticles(R"(C:\Users\Veronika\discrete-elements\auxiliary\converted.txt)", constantsFile);

    GridCell::defaultSize = Particle::maxRadius * 2;
	Grid grid(workspace);
	Particle::setGridCellPositions(system, grid);
	grid.setCellsContents(system);
	Particle::refreshDeltaWall(grid, border);
	log << "Grid is set" << endl;

//    auto edge = getEdge(system, grid);
//    exportEdge(R"(C:\Users\Veronika\Documents\visualisation\edge.txt)", grid, edge);
//    exportGrid(R"(C:\Users\Veronika\Documents\visualisation\grid.txt)", grid);

	vector<Particle>::iterator it;
    double time = 0;
	exportDetails(R"(C:\Users\Veronika\Documents\visualisation\info.txt)", border, system);
	log << "Details are exported" << endl;

	ofstream fout(outputFile);
	ofstream fout_e(outputEnergyFile);

    if(!Particle::isPacked) {
        try {
            time = pack(fout, fout_e, system, grid, timeStep, border);
            log << "Packing is ready" << endl;
            Particle::isPacked = true;
            exportParticles(R"(C:\Users\Veronika\discrete-elements\auxiliary\packed.txt)", system);
        } catch (ParticleOutOfBorderException& e) {
            wentOut = e.what();
        }
    }
    fout << time << " ";
    appendSystemPosition(fout, system);
	if(!packOnly && wentOut == nullptr) {
        setNeighbours(system, grid);
        try {
            //shiftWall(fout, fout_e, system, grid, timeStep, time, border);
            removeWall(fout, fout_e, system, grid, timeStep, time, border);
            fout << time << " ";
            appendSystemPosition(fout, system);
        } catch (ParticleOutOfBorderException& e) {
            wentOut = e.what();
        }
	}
	fout.close();
    fout_e.close();

    if(wentOut != nullptr) {
        log << "One of the particles has crossed the border. Program execution is stopped";
    }

	log << argv[0] << " ended successfully" << endl;
	return 0;
}
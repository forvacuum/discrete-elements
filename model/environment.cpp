#include "environment.h"

Environment::Environment() {
    std::string infoFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\info.txt)";
    std::ifstream fin(infoFile);

    int type;
    border = new double[4];
    timeStep = 0;

    for (int i = 0; i < 4; i++) {
        fin >> border[i];
    }
    fin >> timeStep;
    fin >> type;
    fin.close();
    actionType = static_cast<Action>(type);
    workspace = setWorkspace();

    if(actionType != GENERATE) {
        system = importParticles(particlesFile,
                                 constantsFile);
        GridCell::defaultSize = Particle::maxRadius * 2;
        grid = new Grid(workspace);
        Particle::setGridCellPositions(system, grid);
        grid->setCellsContents(system);
        Particle::refreshDeltaWall(grid, border);
    }
}

void Environment::execute() {
    if(actionType == GENERATE) {
        generateParticlesTriangle(generatorInfoFile, particlesFile, border);
        return;
    }
    Particle* wentOut = nullptr;
    std::ofstream fout(outputFile);
    std::ofstream fout_e(outputEnergyFile);
    exportDetails(R"(C:\Users\Veronika\discrete-elements\visualisation\info.txt)", border, system);
    switch (actionType) {
        case PACK:
            Particle::isPacked = false;
            try {
                pack(fout, fout_e, system, grid, timeStep, border);
                fout << 0 << " ";
                appendSystemPosition(fout, system);
                exportParticles(particlesFile, system);
            } catch (ParticleOutOfBorderException& e) {
                wentOut = e.what();
            }
            break;
        case REMOVE_WALL:
            Particle::isPacked = true;
            setNeighbours(system, grid);
            try {
                removeWall(fout, fout_e, system, grid, timeStep, border);
                fout << 0 << " ";
                appendSystemPosition(fout, system);
                exportParticles(particlesFile, system);
            } catch (ParticleOutOfBorderException& e) {
                wentOut = e.what();
            }
            break;
        case SHIFT_WALL:
            Particle::isPacked = true;
            setNeighbours(system, grid);
            try {
                shiftWall(fout, fout_e, system, grid, timeStep, border);
                fout << 0 << " ";
                appendSystemPosition(fout, system);
                exportParticles(R"(C:\Users\Veronika\discrete-elements\auxiliary\shifted.txt)", system);
            } catch (ParticleOutOfBorderException& e) {
                wentOut = e.what();
            }
            break;
        case GET_EDGE:
            fout << 0 << " ";
            appendSystemPosition(fout, system);
            exportEdge(R"(C:\Users\Veronika\discrete-elements\visualisation\edge.txt)",
                       grid, getEdge(system, grid));
            exportGrid(R"(C:\Users\Veronika\discrete-elements\visualisation\grid.txt)", grid);
        default:
            break;
    }
    fout.close();
    fout_e.close();

    if(wentOut != nullptr) {
        std::cout << "One of the particles has crossed the border. Program execution is stopped" << std::endl;
    }
}

/** Returns the boundaries of the available workspace to set the grid
 *
 * The height of the workspace calculations are based on the assumption
 * that the angle of repose is unlikely to be less than 7° **/
double* Environment::setWorkspace(){
    double borderWidth = border[1] - border[0];

    double extraWidth = borderWidth;
    auto newWorkspace = new double[4];

    newWorkspace[0] = border[0] - extraWidth;
    newWorkspace[1] = border[1] + extraWidth;
    newWorkspace[2] = border[2];
    newWorkspace[3] = border[3];

    return newWorkspace;
}
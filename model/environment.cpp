#include "environment.h"

Environment::Environment() {
    std::ifstream fin(inputInfoFile);

    int type;
    border = new double[4];
    timeStep = 0;

    for (int i = 0; i < 4; i++) {
        fin >> border[i];
    }
    fin >> timeStep;
    fin >> type;
    fin.close();
    action = static_cast<Action>(type);
    workspace = setWorkspace();
}

void Environment::setSystem() {
    if(action != GENERATE) {
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
    if(action == GENERATE) {
        generateParticlesTriangle(generatorInfoFile, particlesFile, border);
        return;
    }
    Particle* wentOut = nullptr;
    std::ofstream fout(outputParticlesFile);
    std::ofstream fout_e(outputEnergyFile);
    exportDetails(outputInfoFile, border, system);
    switch (action) {
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
                exportParticles(particlesFile, system);
            } catch (ParticleOutOfBorderException& e) {
                wentOut = e.what();
            }
            break;
        case GET_EDGE:
            fout << 0 << " ";
            appendSystemPosition(fout, system);
            exportEdge(outputEdgeFile,
                       grid, getEdge(system, grid));
            exportGrid(outputGridFile, grid);
        default:
            break;
    }
    fout.close();
    fout_e.close();

    if(wentOut != nullptr) {
        throw ParticleOutOfBorderException(wentOut);
        //std::cout << "One of the particles has crossed the border. Program execution is stopped" << std::endl;
    }
}

std::string Environment::getDetails() {
    std::string s = "Border: ";
    for (int i = 0; i < 4; i++) {
        s += std::to_string(border[i]) + " ";
    }
    s += "\nTime step: " + std::to_string(timeStep);

    return s;
}

/** Returns the boundaries of the available workspace to set the grid
 *
 * The height of the workspace calculations are based on the assumption
 * that the angle of repose is unlikely to be less than 7° **/
double* Environment::setWorkspace() {
    double borderWidth = border[1] - border[0];

    double extraWidth = borderWidth;
    auto newWorkspace = new double[4];

    newWorkspace[0] = border[0] - extraWidth;
    newWorkspace[1] = border[1] + extraWidth;
    newWorkspace[2] = border[2];
    newWorkspace[3] = border[3];

    return newWorkspace;
}
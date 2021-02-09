#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>
#include "vector2D.h"
#include "particle.h"
#include "energy.h"
#include "action.h"
#include "impexp.h"
#include "calc.h"

struct Environment {
public:
    std::string inputInfoFile = "..\\..\\auxiliary\\info.txt";
    std::string particlesFile = "..\\..\\auxiliary\\particles.txt";
    std::string constantsFile = "..\\..\\auxiliary\\const.txt";
    std::string generatorInfoFile = "..\\..\\auxiliary\\generatorinfo.txt";

    std::string outputInfoFile = "..\\..\\visualisation\\info.txt";
    std::string outputParticlesFile = "..\\..\\visualisation\\output.txt";
    std::string outputEnergyFile = "..\\..\\visualisation\\energy.txt";
    std::string outputEdgeFile = "..\\..\\visualisation\\edge.txt";
    std::string outputGridFile = "..\\..\\visualisation\\grid.txt";


    double* border;
    double* workspace;
    double timeStep;
    Action action;

    std::vector<Particle> system;
    Grid* grid;

    Environment();
    void setSystem();
    void execute();
private:
    double* setWorkspace();
};
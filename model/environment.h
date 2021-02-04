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
    std::string particlesFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\particles.txt)";
    std::string constantsFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\const.txt)";
    std::string generatorInfoFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\generatorinfo.txt)";
    std::string outputFile = R"(C:\Users\Veronika\discrete-elements\visualisation\output.txt)";
    std::string outputEnergyFile = R"(C:\Users\Veronika\discrete-elements\visualisation\energy.txt)";

    double* border;
    double* workspace;
    double timeStep;
    Action actionType;

    std::vector<Particle> system;
    Grid* grid;

    Environment();
    void execute();
private:
    double* setWorkspace();
};
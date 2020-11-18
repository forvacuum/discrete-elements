#include "impexp.h"
#include "parameter.h"

void initialize(double* border, double& timeStep, bool& generatorEnabled, bool& prePacked, bool& packOnly) {
    std::string infoFile = R"(C:\Users\Veronika\discrete-elements\auxiliary\info.txt)";

    std::ifstream fin(infoFile);
    for (int i = 0; i < 4; i++) {
        fin >> border[i];
    }

    fin >> timeStep >> generatorEnabled >> prePacked >> packOnly;
    fin.close();
}

/** Returns the boundaries of the available workspace to set the grid
 *
 * The height of the workspace calculations are based on the assumption
 * that the angle of repose is unlikely to be less than 7° **/
double* setWorkspace(const double border[4]) {
    double borderWidth = border[1] - border[0];

    // 0.122173 radians correspond to 7°
    //double extraWidth = borderHeight / tan(0.122173);
    double extraWidth = 2 * borderWidth;
    auto* workspace = new double[4];

    workspace[0] = border[0] - extraWidth;
    workspace[1] = border[1] + extraWidth;
    workspace[2] = border[2];
    workspace[3] = border[3];

    return workspace;
}
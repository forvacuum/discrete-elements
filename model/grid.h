#pragma once
#include <vector>
#include <algorithm>
#include "particle.h"

struct Grid;
struct Particle;

//TODO: implement the cell access by its coordinates
//      fix the coordinates calculation or remove these fields
struct GridCell {
	double x;
	double y;
	std::vector<Particle*> contents;

	static double defaultSize;
	static double width;
	static double height;

    GridCell();
	GridCell(double, double);
	void addParticle(Particle* particle);
	void removeParticle(Particle* particle);
	static double getPreferredSize(double workspaceSize);
};

// Add particles system as a class member
struct Grid {
    std::vector<GridCell> grid;
    size_t horizontalAmount;
    size_t verticalAmount;
    double* workspaceBorder;

    Grid(double* workspace);
    GridCell& at(size_t index);
    void setCellsContents(std::vector<Particle>& system);

private:
    std::vector<GridCell> setGrid(const double workspace[4]);
};
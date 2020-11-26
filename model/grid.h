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
	void addParticle(Particle*);
	void removeParticle(Particle*);
	static void setCellsContents(Grid&, std::vector<Particle>&);
	static double getPreferredSize(double);
};

struct Grid {
    std::vector<GridCell> grid;
    size_t horizontalAmount;
    size_t verticalAmount;
    double* workspaceBorder;

    Grid(double*);
    GridCell& at(size_t);

private:
    std::vector<GridCell> setGrid(const double[4]);
};
#pragma once
#include <vector>
#include <algorithm>
#include "particle.h"

struct Particle;

struct GridCell {
	double x;
	double y;
	std::vector<Particle*> contents;

	static double defaultSize;
	static double length;
	static double width;

	static size_t horizontalAmount;
	static size_t verticalAmount;


	GridCell();
	GridCell(double, double);
	void addParticle(Particle*);
	void removeParticle(Particle*);
	static std::vector<GridCell> setGrid(double[4]);
	static void setCellsContents(std::vector<GridCell>&, std::vector<Particle>&);

private:
	static double getPreferredSize(double);
};
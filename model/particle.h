#pragma once
#include "vector2D.h"
#include "grid.h"
#include <vector>

struct GridCell;

struct Particle {
	Vector position;
	Vector velocity;
	int gridRow; // left to right
	int gridColumn; // bottom to top
	//size_t gridCellIndex;

	double radius;
	double mass;

	//std::vector<double> delta;
	double deltaWall[4];

	static double stiffnessRepulsive;
	static double stiffnessAttractive;
	static double maxRadius;
	static double criticalDistance;
	static bool isWallEnabled[4];
	static bool isPacked;

	Particle();
	bool operator==(const Particle&) const;
	bool operator!=(const Particle&) const;
	void refreshGridCoordinates(std::vector<GridCell>&);
	static void setGridCellPositions(std::vector<Particle>&, const std::vector<GridCell>&);
	static void refreshDeltaWall(std::vector<GridCell>&, const double[4]);
};
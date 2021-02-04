#pragma once
#include "grid.h"
#include "environment.h"
#include "vector2D.h"
#include <vector>
#include <unordered_map>

struct GridCell;

struct Particle {
	Vector position;
	Vector velocity;
	int gridRow; // left to right
	int gridColumn; // bottom to top

	//TODO: replace vector with an array
    //std::unordered_map<Particle*, double> neighbour;
    std::vector<Particle*> neighbour;

    /* Contains the shear force value on the previous step for each neighbouring particle */
    std::unordered_map<Particle*, double> shearForceValue;

	double radius;
	double mass;

	double deltaWall[4]{};

	static double stiffnessRepulsive;
	static double stiffnessAttractive;
	static double stiffnessShear;
	static double particleFriction;
	static double wallFriction;
	static double floorFriction;
	static double minRadius;
	static double maxRadius;
	static double criticalDistance;
	static bool isWallEnabled[4];
	static bool isPacked;

	Particle();
	bool operator==(const Particle& particle) const;
	bool operator!=(const Particle& particle) const;
	void refreshGridCoordinates(Grid *grid);
    static void setGridCellPositions(std::vector<Particle>& system, Grid* grid);
    static void refreshDeltaWall(Grid* grid, const double border[4]);
};
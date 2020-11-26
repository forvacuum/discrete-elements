#pragma once
#include "grid.h"
#include "parameter.h"
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
	bool operator==(const Particle&) const;
	bool operator!=(const Particle&) const;
	void refreshGridCoordinates(Grid&, const double[4]);
    static void setGridCellPositions(std::vector<Particle>&, Grid& grid);
    static void refreshDeltaWall(Grid&, const double[4]);
};
#pragma once
#include "vector2D.h"
#include "grid.h"
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
	static double frictionCoefficient;
	static double minRadius;
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
	//static std::string getConstants();
};
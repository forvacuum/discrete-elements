#pragma once
#include "vector2D.h"
#include <vector>

struct Particle {
	Vector position;
	Vector velocity;

	double radius;
	double mass;
	double stiffness;

	std::vector<double> delta;
	double deltaWall[4];

	bool operator==(const Particle&) const;
	bool operator!=(const Particle&) const;
};
#pragma once
#include "vector2D.h"
#include <vector>

struct Particle {
	Vector position;
	Vector velocity;

	double radius;
	double mass;
	double stiffness;

	bool operator==(const Particle& p) const {
		return this == &p;
	}
};
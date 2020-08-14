#include "force.h"

Vector applyWeight(const Particle& p) {
	return (- p.mass * g * Vector(0, 1));
}

Vector applyRepulsion(const Particle& p, const std::vector<Particle>& system) {
	Vector resultant = Vector();
	Vector relativePosition;
	Vector n;
	auto it = system.begin();
	double currentNorm;
	double delta;

	while (it != system.end()) {
		relativePosition = p.position - it->position;
		currentNorm = Vector::norm(relativePosition);
		delta = p.radius + it->radius - currentNorm;
		if (p == *it || delta <= 0) {
			it++;
			continue;
		}

		n = relativePosition * (1 / currentNorm);
		resultant += p.stiffness * delta * n;
		it++;
	}

	return resultant;
}

Vector applyWallRepulsion(const Particle& p, const double border[4]) {
	Vector resultant = Vector();
	Vector duplicatedBasis[4] = { Vector(1, 0), Vector(1, 0),
								Vector(0, 1), Vector(0, 1) };
	double duplicatedCoordinate[4] = { p.position.getX(), p.position.getX(),
										p.position.getY(), p.position.getY() };
	double deltaBorder[4];

	for (int i = 0; i < 4; i++) {
		deltaBorder[i] = p.radius - pow(-1, i) * (duplicatedCoordinate[i] - border[i]);
		if (deltaBorder[i] <= 0) {
			continue;
		}
		resultant += pow(-1, i) * duplicatedBasis[i] * p.stiffness * deltaBorder[i];
	}

	return resultant;
}
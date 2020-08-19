#include "force.h"

Vector applyWeight(const Particle& p) {
	return (- p.mass * g * Vector(0, 1));
}

Vector applyRepulsion(const Particle& p, const std::vector<Particle>& system) {
	Vector resultant = Vector();
	Vector relativePosition;
	Vector n;
	auto it = system.begin();
	unsigned counter = 0;

	while (it != system.end()) {
		relativePosition = p.position - it->position;
		if (*it != p) {
			n = relativePosition * (1 / Vector::norm(relativePosition));
			resultant += p.stiffness * p.delta[counter] * n;
		}
		counter++;
		it++;
	}

	return resultant;
}

Vector applyWallRepulsion(const Particle& p, const double border[4]) {
	Vector resultant = Vector();
	Vector duplicatedBasis[4] = { Vector(1, 0), Vector(1, 0),
								Vector(0, 1), Vector(0, 1) };

	for (int i = 0; i < 4; i++) {
		resultant += pow(-1, i) * duplicatedBasis[i] * p.stiffness * p.deltaWall[i];
	}

	return resultant;
}

Vector applyDissipation(const Particle& p) {
	return - dampingCoefficient * Vector(p.velocity);
}
#include "force.h"

Vector applyWeight(const Particle& p) {
	return (- p.mass * g * Vector(0, 1));
}

Vector applyRepulsion(const Particle& p, std::vector<GridCell>& grid) {
	Vector resultant = Vector();
	Vector relativePosition;
	Vector n;
	size_t cellIndex;
	double delta;

	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;

	for (int i = p.gridRow - 1; i <= p.gridRow + 1; i++) {
		if (i >= 0 && i < GridCell::horizontalAmount) {
			for (int j = p.gridColumn - 1; j <= p.gridColumn + 1; j++) {
				if (j >= 0 && j < GridCell::horizontalAmount) {
					cellIndex = i * GridCell::verticalAmount + j;

					it = grid.at(cellIndex).contents.begin();
					lastParticle = grid.at(cellIndex).contents.end();
					
					while (it != lastParticle) {
						relativePosition = p.position - (*it)->position;
						n = relativePosition * (1 / Vector::norm(relativePosition));
						delta = p.radius + (*it)->radius - Vector::norm(relativePosition);

						if (delta < 0 || *it == &p) {
							it++;
							continue;
						}

						resultant += Particle::stiffness * delta * n;
						it++;
					}
				}
			}
		}
	}

	return resultant;
}

Vector applyWallRepulsion(const Particle& p, const double border[4]) {
	Vector resultant = Vector();
	Vector duplicatedBasis[4] = { Vector(1, 0), Vector(1, 0),
								Vector(0, 1), Vector(0, 1) };

	for (int i = 0; i < 4; i++) {
		if (Particle::isWallEnabled[i]) {
			resultant += pow(-1, i) * duplicatedBasis[i] * Particle::stiffness * p.deltaWall[i];
		}
	}

	return resultant;
}

Vector applyDissipation(const Particle& p) {
	return - dampingCoefficient * Vector(p.velocity);
}
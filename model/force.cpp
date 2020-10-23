#include "force.h"

Vector applyWeight(const Particle& p) {
	return (- p.mass * g * Vector(0, 1));
}

Vector applyNormalForce(const Particle& p, std::vector<GridCell>& grid) {
	Vector resultant = Vector();
	Vector relativePosition;
	Vector n;
	size_t cellIndex;
	double delta;

	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;

	/* Iterating through all particles in adjacent grid cells */
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

						if (*it == &p) {
							it++;
							continue;
						}
						if (delta >= 0) {
							resultant += Particle::stiffnessRepulsive * delta * n;
						}
						else if (Particle::isPacked && (-delta) < Particle::criticalDistance) {
							//delta here is a negative value so the force direction will be reversed
							resultant += Particle::stiffnessAttractive * delta * n;
						}

						it++;
					}
				}
			}
		}
	}

	return resultant;
}

Vector applyShearForce(Particle& p, std::vector<GridCell>& grid, double timestep) {
	Vector resultant = Vector();
	Vector relativePosition;
	Vector relativeVelocity;
	Vector relativeShearVelocity;
	Vector n, t;
	size_t cellIndex;
	double deltaNormal, deltaShear;
	double shearForce = 0;
	double normalForce = 0;

	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;
	std::unordered_map<Particle*, double>::const_iterator map_it;

	/* This force is defined only for packed particles */
	if (!Particle::isPacked) {
		return resultant;
	}

	/* Iterating through all particles in adjacent grid cells */
	for (int i = p.gridRow - 1; i <= p.gridRow + 1; i++) {
		if (i >= 0 && i < GridCell::horizontalAmount) { // checking for row index correctness
			for (int j = p.gridColumn - 1; j <= p.gridColumn + 1; j++) {
				if (j >= 0 && j < GridCell::horizontalAmount) { // checking for column index correctness
					cellIndex = i * GridCell::verticalAmount + j; 

					it = grid.at(cellIndex).contents.begin();
					lastParticle = grid.at(cellIndex).contents.end();
					
					/* Iterating through all particles in the current cell */
					while (it != lastParticle) {
						relativePosition = p.position - (*it)->position;
						n = relativePosition * (1 / Vector::norm(relativePosition));
						deltaNormal = p.radius + (*it)->radius - Vector::norm(relativePosition);

						if (*it == &p) { // excluding the case of particle interaction with itself
							it++;
							continue;
						}
						if (deltaNormal > 0) {
							relativeVelocity = -((*it)->velocity - p.velocity);
							relativeShearVelocity = relativeVelocity - Vector::dotProduct(relativeVelocity, n) * n;
							if (Vector::norm(relativeShearVelocity) != 0) {
								t = relativeShearVelocity * (1 / Vector::norm(relativeShearVelocity));
								deltaShear = Vector::norm(relativeShearVelocity) * timestep;
								normalForce = Particle::stiffnessRepulsive * deltaNormal;

								map_it = p.shearForceAbsolute.find(*it);
								if (map_it != p.shearForceAbsolute.end()) { //current particle has already been touched
									shearForce = map_it->second;
									shearForce += Particle::stiffnessShear * deltaShear;

									if (shearForce > Particle::frictionCoefficient * normalForce) { //shear force exceeds required value
										shearForce = Particle::frictionCoefficient * normalForce;
									}

									p.shearForceAbsolute.at(map_it->first) = shearForce;
								}
								else { //current particle is the new one
									shearForce = Particle::stiffnessShear * deltaShear;

									if (shearForce > Particle::frictionCoefficient * normalForce) { //shear force exceeds required value
										shearForce = Particle::frictionCoefficient * normalForce;
									}

									p.shearForceAbsolute.insert(std::make_pair(*it, shearForce));
								}
								resultant += -shearForce * t;
							}
						}
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
			resultant += pow(-1, i) * duplicatedBasis[i] * Particle::stiffnessRepulsive * p.deltaWall[i];
		}
	}

	return resultant;
}

Vector applyDissipation(const Particle& p) {
	return - dampingCoefficient * Vector(p.velocity);
}
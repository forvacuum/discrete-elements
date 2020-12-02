#include "calc.h"
#include "impexp.h"
#include "energy.h"
#include "force.h"

Vector applyWeight(const Particle& p) {
	return (- p.mass * g * Vector(0, 1));
}

Vector applyNormalForce(Particle& p, Grid& grid) {
	Vector resultant = Vector();
	Vector relativePosition;
	Vector n;
	size_t cellIndex;
	double delta;

	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;
    std::vector<Particle*>::iterator it_tmp;

	/* Iterating through all particles in adjacent grid cells */
	for (int i = p.gridRow - 1; i <= p.gridRow + 1; i++) {
		if (i >= 0 && i < grid.horizontalAmount) {
			for (int j = p.gridColumn - 1; j <= p.gridColumn + 1; j++) {
				if (j >= 0 && j < grid.verticalAmount) {
					cellIndex = i * grid.verticalAmount + j;

					it = grid.at(cellIndex).contents.begin();
					lastParticle = grid.at(cellIndex).contents.end();

					while (it != lastParticle) { // Excluding the particle itself
					    if (*it == &p) {
                            it++;
                            continue;
                        }

						relativePosition = p.position - (*it)->position;
						n = relativePosition * (1 / Vector::norm(relativePosition));
						delta = p.radius + (*it)->radius - Vector::norm(relativePosition);

						// Particles are in contact -> the force is repulsive
						if (delta >= 0) {
							resultant += Particle::stiffnessRepulsive * delta * n;
						}
						/* Attractive force
						 * Only for packed system
						 * Only for initially neighbouring particles
						 * Acting only at low distances
						 * */
						else if (Particle::isPacked && (-delta) < Particle::criticalDistance
						&& std::find(p.neighbour.begin(), p.neighbour.end(), *it) != p.neighbour.end()) {
							//delta here is a negative value so the force direction will be reversed
							resultant += Particle::stiffnessAttractive * delta * n;
						}
						// Removing neighbour when the distance exceeds some critical value
						else if( (-delta) >= Particle::criticalDistance
                                 && (it_tmp = std::find(p.neighbour.begin(), p.neighbour.end(), *it)) != p.neighbour.end()) {
						    p.neighbour.erase(it_tmp);
						}
						it++;
					}
				}
			}
		}
	}
	return resultant;
}

Vector applyShearForce(Particle& p, Grid& grid, double timeStep) {
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

//	 This force is defined only for packed particles
	if (!Particle::isPacked) {
		return resultant;
	}

//	 Iterating through all particles in adjacent grid cells
	for (int i = p.gridRow - 1; i <= p.gridRow + 1; i++) {
		if (i >= 0 && i < grid.horizontalAmount) { // checking for row index correctness
			for (int j = p.gridColumn - 1; j <= p.gridColumn + 1; j++) {
				if (j >= 0 && j < grid.verticalAmount) { // checking for column index correctness
					cellIndex = i * grid.verticalAmount + j;

					it = grid.at(cellIndex).contents.begin();
					lastParticle = grid.at(cellIndex).contents.end();
					
					// Iterating through all particles in the current cell
					while (it != lastParticle) {
						relativePosition = p.position - (*it)->position;
						n = relativePosition * (1 / Vector::norm(relativePosition));
						deltaNormal = p.radius + (*it)->radius - Vector::norm(relativePosition);

                        // Excluding the case of particle interaction with itself
						if (*it == &p) {
							it++;
							continue;
						}

                        // Necessary condition for shear force arising
						if (deltaNormal > 0) {
							relativeVelocity = -((*it)->velocity - p.velocity);
							relativeShearVelocity = relativeVelocity - Vector::dotProduct(relativeVelocity, n) * n;
							if (Vector::norm(relativeShearVelocity) != 0) { //excluding the case of zero division
								t = relativeShearVelocity * (1 / Vector::norm(relativeShearVelocity));
								deltaShear = Vector::norm(relativeShearVelocity) * timeStep;
								normalForce = Particle::stiffnessRepulsive * deltaNormal;

								map_it = p.shearForceValue.find(*it);

                                // Current particle has already been touched on a previous step
								if (map_it != p.shearForceValue.end()) {
									shearForce = map_it->second;
									shearForce += Particle::stiffnessShear * deltaShear;

									// Shear force exceeds required value
									if (shearForce > Particle::particleFriction * normalForce) {
										shearForce = Particle::particleFriction * normalForce;
									}

									p.shearForceValue.at(map_it->first) = shearForce;
								}

								// Current particle is the new one
								else {
									shearForce = Particle::stiffnessShear * deltaShear;

									// Shear force exceeds required value
									if (shearForce > Particle::particleFriction * normalForce) {
										shearForce = Particle::particleFriction * normalForce;
									}

									p.shearForceValue.insert(std::make_pair(*it, shearForce));
								}

								resultant += -shearForce * t;
							}
						}

						// When two given particles do not contact anymore
						else {
                            map_it = p.shearForceValue.find(*it);
                            // Current particle has to be excluded from the map
                            //      if it stopped affecting on the initial one
                            if (map_it != p.shearForceValue.end()) {
                                p.shearForceValue.erase(map_it);
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

Vector applyWallRepulsion(const Particle &p) {
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

Vector applyWallFriction(const Particle &p) {
    double frictionCoefficient = Particle::wallFriction;

    Vector e;
    Vector resultant = Vector();
    //TODO: make the variable name more relevant
    Vector duplicatedBasis[4] = { Vector(0, 1), Vector(0, 1),
                                  Vector(1, 0), Vector(1, 0) };

    if(!Particle::isPacked) {
        return resultant;
    }
    for (size_t i = 0; i < 4; i++) {
        if(i == 2) { // if current wall is the bottom one
            frictionCoefficient = Particle::floorFriction;
        }
        if(Particle::isWallEnabled[i] && p.deltaWall[i] > 0 && Vector::norm(p.velocity) != 0) {
            //project the current velocity vector to a contact plane
            e = Vector::dotProduct(p.velocity, duplicatedBasis[i]) * duplicatedBasis[i];
            //normalization and redirecting
            e = - p.velocity * (1 / Vector::norm(p.velocity));
            resultant += frictionCoefficient * Particle::stiffnessRepulsive * p.deltaWall[i] * e;
        }
    }
    return resultant;
}

Vector applyDissipation(const Particle& p) {
	return - dampingCoefficient * Vector(p.velocity);
}

Vector applyForce(Particle& particle, Grid& grid, const double border[4], double timeStep) {
	Vector resultant = Vector();

	resultant += applyWeight(particle);
	resultant += applyNormalForce(particle, grid);
	resultant += applyShearForce(particle, grid, timeStep);
	resultant += applyWallRepulsion(particle);
	resultant += applyWallFriction(particle);
	resultant += applyDissipation(particle);

	return resultant;
}
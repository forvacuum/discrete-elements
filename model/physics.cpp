#include "physics.h"
#include "impexp.h"

double pack(std::ofstream& fout, std::ofstream& fout_e, std::vector<Particle>& system,
          Grid& grid, double timeStep, const double border[4]) {
    double packTime = 0;
    //double positionDiff;
    double energyDiff;
    //double systemPositionNorm = 0;
    double systemEnergy = 0;
    double eps = 1e-7;
    //TODO: switch the stopping criterion onto energy relation
    do {
        //fout << packTime << " ";
        //positionDiff = systemPositionNorm;
        energyDiff = systemEnergy;
        //appendSystemPosition(fout, system);
        //systemEnergy = appendSystemEnergy(fout_e, system, grid, border);
        systemEnergy = calculateTotalSystemEnergy(system, border, grid);
        calculateNextIteration(system, grid, timeStep, border);
        //systemPositionNorm = totalPositionNorm(system);
        //positionDiff -= systemPositionNorm;
        energyDiff -= systemEnergy;
        packTime += timeStep;

    } while (abs(energyDiff) >= eps || packTime < 1);

    return packTime;
}

void setNeighbours(std::vector<Particle>& system, Grid& grid) {
    size_t cellIndex;
    double delta;
    Vector relativePosition;
    Vector n;

    auto it = system.begin();
    std::vector<Particle*>::iterator itGrid;
    std::vector<Particle*>::iterator lastParticle;

    while (it != system.end()) {
        for (int i = it->gridRow - 1; i <= it->gridRow + 1; i++) {
            if (i >= 0 && i < grid.horizontalAmount) { // checking for row index correctness
                for (int j = it->gridColumn - 1; j <= it->gridColumn + 1; j++) {
                    if (j >= 0 && j < grid.horizontalAmount) { // checking for column index correctness
                        cellIndex = i * grid.verticalAmount + j;

                        itGrid = grid.at(cellIndex).contents.begin();
                        lastParticle = grid.at(cellIndex).contents.end();

                        while (itGrid != lastParticle) {
                            if (*itGrid == &(*it)) {
                                itGrid++;
                                continue;
                            }
                            relativePosition = it->position - (*itGrid)->position;
                            delta = it->radius + (*itGrid)->radius - Vector::norm(relativePosition);
                            if(delta > 0) {
                                it->neighbour.push_back(*itGrid);
                            }
                            itGrid++;
                        }
                    }
                }
            }
        }
        it++;
    }
}

double execute(std::ofstream& fout, std::ofstream& fout_e, std::vector<Particle>& system,
            Grid& grid, double timeStep, double packTime, const double border[4]) {
    double workTime = 0;
    double totalTime = packTime;
    double energyDiff;
    double systemEnergy = 0;
    double eps = 1e-7;

    do {
        //fout << totalTime << " ";
        //appendSystemPosition(fout, system);
        //energyDiff = systemEnergy;
        //systemEnergy = appendSystemEnergy(fout_e, system, grid, border);
        //systemEnergy = calculateTotalSystemEnergy(system, border, grid);
        calculateNextIteration(system, grid, timeStep, border);
        //energyDiff -= systemEnergy;
        workTime += timeStep;
        totalTime += timeStep;

    } while (abs(calculateEnergyRelation(system, border)) >= eps || workTime < 1);

    return workTime;
}

// Note: force function is considered not to depend on time explicitly
void calculatePosition(std::vector<Particle>& system, Grid& grid,
                       double timeStep, const double border[4]) {
	Vector force;
	auto it = system.begin();

	while (it != system.end()) {

		force = applyForce(*it, grid, border, timeStep);

		it->position.setX(it->position.getX() + it->velocity.getX() * timeStep +
                          (force.getX() / 2 / it->mass) * timeStep * timeStep );
		it->position.setY(it->position.getY() + it->velocity.getY() * timeStep +
                          (force.getY() / 2 / it->mass) * timeStep * timeStep );

		//intermediate velocity
		it->velocity.setX(it->velocity.getX() + timeStep * force.getX() / 2 / it->mass);
		it->velocity.setY(it->velocity.getY() + timeStep * force.getY() / 2 / it->mass);

		it->refreshGridCoordinates(grid, border);

		it++;
	}

	Particle::refreshDeltaWall(grid, border);

}

void calculateVelocity(std::vector<Particle>& system, Grid& grid,
                       double timeStep, const double border[4]) {
	Vector force;
	auto it = system.begin();

	while (it != system.end()) {

		//velocity on the next step
		force = applyForce(*it, grid, border, timeStep); //updating the force vector
		it->velocity.setX(it->velocity.getX() + timeStep * force.getX() / 2 / it->mass);
		it->velocity.setY(it->velocity.getY() + timeStep * force.getY() / 2 / it->mass);

		it++;
	}
}

void calculateNextIteration(std::vector<Particle>& system, Grid& grid, double timeStep, const double border[4]) {
	calculatePosition(system, grid, timeStep, border);
	calculateVelocity(system, grid, timeStep, border);
}


Vector applyForce(Particle& particle, Grid& grid, const double border[4], double timeStep) {
	Vector resultant = Vector();

	resultant += applyWeight(particle);
	resultant += applyNormalForce(particle, grid);
	resultant += applyShearForce(particle, grid, timeStep);
	resultant += applyWallRepulsion(particle, border);
	resultant += applyWallFriction(particle, border);
	resultant += applyDissipation(particle);

	return resultant;
}

double calculateEnergyRelation(const std::vector<Particle>& system, const double border[4]) {
    auto it = system.begin();
    double result = 0;
    double kinetic = 0;
    double potential = 0;

    while (it != system.end()) {
        kinetic += kineticEnergy(*it);
        potential += potentialEnergy(*it, border);
        it++;
    }

    return kinetic / potential;
}

double calculateTotalEnergy(const Particle& particle, const double border[4], Grid& grid) {
	double totalEnergy = 0;

	/* Kinetic */
	totalEnergy += kineticEnergy(particle);

	/* Potential (weight) */
	totalEnergy += potentialEnergy(particle, border); 

	/* Elastic (particles) */
	totalEnergy += normalForceEnergy(particle, grid);
	
	//totalEnergy += shearForceEnergy(particle);

	/* Elastic (walls) */
	totalEnergy += elasticWallEnergy(particle);

	return totalEnergy;
}

double calculateTotalSystemEnergy(const std::vector<Particle>& system, const double border[4], Grid& grid) {
    auto it = system.begin();
    double systemEnergy = 0;
    double tmp;

    while (it != system.end()) {
        tmp = calculateTotalEnergy(*it, border, grid);
        systemEnergy += tmp;
        it++;
    }

    return systemEnergy;
}

double kineticEnergy(const Particle& particle) {
	return particle.mass * Vector::dotProduct(particle.velocity, particle.velocity) / 2;
}

double potentialEnergy(const Particle& particle, const double border[4]) {
	return particle.mass * g * (particle.position.getY() - border[2]);  //reference level is considered to be exactly the bottom border line
}

double normalForceEnergy(const Particle& particle, Grid& grid) {
	double result = 0;
	double delta = 0;
	size_t cellIndex;
	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;

	for (int i = particle.gridRow - 1; i <= particle.gridRow + 1; i++) {
		if (i >= 0 && i < grid.horizontalAmount) {
			for (int j = particle.gridColumn - 1; j <= particle.gridColumn + 1; j++) {
				if (j >= 0 && j < grid.horizontalAmount) {
					cellIndex = i * grid.verticalAmount + j;

					it = grid.at(cellIndex).contents.begin();
					lastParticle = grid.at(cellIndex).contents.end();

					while (it != lastParticle) {
						delta = particle.radius + (*it)->radius - Vector::norm(particle.position - (*it)->position);

						if (*it == &particle) {
							it++;
							continue;
						}

						if (delta > 0) {
							result += Particle::stiffnessRepulsive * delta * delta / 4;
						}
						else if (Particle::isPacked && (-delta) < Particle::criticalDistance) {
							result += Particle::stiffnessAttractive * delta * delta / 4;
						}

						it++;
					}
				}
			}
		}
	}

	return result;
}

//double shearForceEnergy(const Particle& particle) {
//	double result = 0;
//	double delta;
//	auto it = particle.shearForceAbsolute.begin();
//
//	if (Particle::stiffnessShear != 0)
//	while (it != particle.shearForceAbsolute.end()) {
//		//if ()
//		delta = it->second / Particle::stiffnessShear;
//		result += Particle::stiffnessShear ;
//	}
//}

double elasticWallEnergy(const Particle& particle) {
	double result = 0;

	for (int i = 0; i < 4; i++) {
		result += Particle::stiffnessRepulsive * particle.deltaWall[i] * particle.deltaWall[i] / 2;
	}

	return result;
}


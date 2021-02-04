#include "energy.h"
#include "impexp.h"
#include "calc.h"
#include "force.h"


double calculateTotalEnergy(const Particle& particle, const double border[4], Grid* grid) {
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

double calculateTotalSystemEnergy(const std::vector<Particle>& system, const double border[4], Grid* grid) {
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

double normalForceEnergy(const Particle& particle, Grid* grid) {
	double result = 0;
	double delta = 0;
	size_t cellIndex;
	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;

	for (int i = particle.gridRow - 1; i <= particle.gridRow + 1; i++) {
		if (i >= 0 && i < grid->horizontalAmount) {
			for (int j = particle.gridColumn - 1; j <= particle.gridColumn + 1; j++) {
				if (j >= 0 && j < grid->verticalAmount) {
					cellIndex = i * grid->verticalAmount + j;

					it = grid->at(cellIndex).contents.begin();
					lastParticle = grid->at(cellIndex).contents.end();

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


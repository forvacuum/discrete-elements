#include "physics.h"

// Note: force function is considered not to be dependent on time explicitly
void calculatePosition(std::vector<Particle>& system, std::vector<GridCell>& grid,
						double timestep, const double border[4]) {
	Vector force;
	auto it = system.begin();

	while (it != system.end()) {

		force = applyForce(*it, grid, border);

		it->position.setX( it->position.getX() + it->velocity.getX() * timestep +
							(force.getX() / 2 / it->mass) * timestep * timestep );
		it->position.setY( it->position.getY() + it->velocity.getY() * timestep +
							(force.getY() / 2 / it->mass) * timestep * timestep );

		//intermediate velocity
		it->velocity.setX(it->velocity.getX() + timestep * force.getX() / 2 / it->mass);
		it->velocity.setY(it->velocity.getY() + timestep * force.getY() / 2 / it->mass);

		it->refreshGridCoordinates(grid);

		it++;
	}

	Particle::refreshDeltaWall(grid, border);

}


void calculateVelocity(std::vector<Particle>& system, std::vector<GridCell>& grid,
						double timestep, const double border[4]) {
	Vector force;
	auto it = system.begin();

	while (it != system.end()) {

		//velocity on the next step
		force = applyForce(*it, grid, border); //updating the force vector
		it->velocity.setX(it->velocity.getX() + timestep * force.getX() / 2 / it->mass);
		it->velocity.setY(it->velocity.getY() + timestep * force.getY() / 2 / it->mass);

		it++;
	}
}

void calculateNextIteration(std::vector<Particle>& system, std::vector<GridCell>& grid, double timestep, const double border[4]) {
	calculatePosition(system, grid, timestep, border);
	calculateVelocity(system, grid, timestep, border);
}


Vector applyForce(const Particle& particle, std::vector<GridCell>& grid, const double border[4]) {
	Vector resultant = Vector();

	resultant += applyWeight(particle);
	resultant += applyNormalForce(particle, grid);
	resultant += applyWallRepulsion(particle, border);
	resultant += applyDissipation(particle);

	return resultant;
}

double calculateTotalEnergy(const Particle& particle, const double border[4], std::vector<GridCell>& grid) {
	double totalEnergy = 0;

	/* Kinetic */
	totalEnergy += kineticEnergy(particle);

	/* Potential (weight) */
	totalEnergy += potentialEnergy(particle, border); 

	/* Elastic (particles) */
	totalEnergy += elasticParticleEnergy(particle, grid);
	
	/* Elastic (walls) */
	totalEnergy += elasticWallEnergy(particle);

	return totalEnergy;
}

double kineticEnergy(const Particle& particle) {
	return particle.mass * Vector::dotProduct(particle.velocity, particle.velocity) / 2;
}

double potentialEnergy(const Particle& particle, const double border[4]) {
	return particle.mass * g * (particle.position.getY() - border[2]);  //reference level is consdered to be exactly the bottom border line
}

double elasticParticleEnergy(const Particle& particle, std::vector<GridCell>& grid) {
	double result = 0;
	double delta = 0;
	size_t cellIndex;
	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator lastParticle;

	for (int i = particle.gridRow - 1; i <= particle.gridRow + 1; i++) {
		if (i >= 0 && i < GridCell::horizontalAmount) {
			for (int j = particle.gridColumn - 1; j <= particle.gridColumn + 1; j++) {
				if (j >= 0 && j < GridCell::horizontalAmount) {
					cellIndex = i * GridCell::verticalAmount + j;

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

double elasticWallEnergy(const Particle& particle) {
	double result = 0;

	for (int i = 0; i < 4; i++) {
		result += Particle::stiffnessRepulsive * particle.deltaWall[i] * particle.deltaWall[i] / 2;
	}

	return result;
}


#include "physics.h"

// Note: force function is considered not to be dependent on time explicitly
void calculatePosition(std::vector<Particle>& system, double timestep, const double border[4]) {
	Vector force;
	auto it = system.begin();

	while (it != system.end()) {
		force = applyForce(*it, system, border);

		it->position.setX( it->position.getX() + it->velocity.getX() * timestep +
							(force.getX() / 2 / it->mass) * timestep * timestep );
		it->position.setY( it->position.getY() + it->velocity.getY() * timestep +
							(force.getY() / 2 / it->mass) * timestep * timestep );

		//intermediate velocity
		it->velocity.setX(it->velocity.getX() + timestep * force.getX() / 2 / it->mass);
		it->velocity.setY(it->velocity.getY() + timestep * force.getY() / 2 / it->mass);

		it++;
	}
	refreshDelta(system, border);
}


void calculateVelocity(std::vector<Particle>& system, double timestep, const double border[4]) {
	Vector force;
	auto it = system.begin();

	while (it != system.end()) {

		//velocity on the next step
		force = applyForce(*it, system, border); //updating the force vector
		it->velocity.setX(it->velocity.getX() + timestep * force.getX() / 2 / it->mass);
		it->velocity.setY(it->velocity.getY() + timestep * force.getY() / 2 / it->mass);

		it++;
	}
}

void calculateNextIteration(std::vector<Particle>& system, double timestep, const double border[4]) {
	calculatePosition(system, timestep, border);
	calculateVelocity(system, timestep, border);
}


Vector applyForce(const Particle& particle, const std::vector<Particle>& system, const double border[4]) {
	Vector resultant = Vector();

	resultant += applyWeight(particle);
	resultant += applyRepulsion(particle, system);
	resultant += applyWallRepulsion(particle, border);
	resultant += applyDissipation(particle);

	return resultant;
}

double calculateTotalEnergy(const Particle& particle, const std::vector<Particle>& system, const double border[4]) {
	double totalEnergy = 0;

	/* Kinetic */
	totalEnergy += kineticEnergy(particle);

	/* Potential (weight) */
	totalEnergy += potentialEnergy(particle, border); 

	/* Elastic (particles) */
	totalEnergy += elasticParticleEnergy(particle, system);
	
	/* Elastic (walls) */
	totalEnergy += elasticWallEnergy(particle, border);

	return totalEnergy;
}

double kineticEnergy(const Particle& particle) {
	return particle.mass * Vector::dotProduct(particle.velocity, particle.velocity) / 2;
}

double potentialEnergy(const Particle& particle, const double border[4]) {
	return particle.mass * g * (particle.position.getY() - border[2]);  //reference level is consdered to be exactly the bottom border line
}

double elasticParticleEnergy(const Particle& particle, const std::vector<Particle>& system) {
	double result = 0;
	double delta;
	auto it = system.begin();

	while (it != system.end()) {
		delta = particle.radius + it->radius - Vector::norm(particle.position - it->position);
		if (delta <= 0 || particle == *it) {
			it++;
			continue;
		}

		result += particle.stiffness * delta * delta / 4;
		it++;
	}

	return result;
}

double elasticWallEnergy(const Particle& particle, const double border[4]) {
	double result = 0;
	double delta;
	double duplicatedCoordinate[4] = { particle.position.getX(), particle.position.getX(),
									particle.position.getY(), particle.position.getY() };

	for (int i = 0; i < 4; i++) {
		delta = particle.radius - pow(-1, i) * (duplicatedCoordinate[i] - border[i]);

		if (delta <= 0) {
			continue;
		}

		result += particle.stiffness * delta * delta / 2;
	}

	return result;
}

void refreshDelta(std::vector<Particle>& system, const double border[4]) {
	size_t size;
	double duplicatedCoordinate[4];
	Vector relativePosition;

	size = system.size();

	for (size_t i = 0; i < size; i++) {
		system[i].delta[i] = 0;
		for (size_t j = 0; j < i; j++) {
			relativePosition = system[i].position - system[j].position;
			system[i].delta[j] = system[i].radius + system[j].radius - Vector::norm(relativePosition);
			if (system[i].delta[j] < 0) {
				system[i].delta[j] = 0;
			}
			system[j].delta[i] = system[i].delta[j];
		}
	}

	for (size_t i = 0; i < size; i++) {
		duplicatedCoordinate[0] = system[i].position.getX();
		duplicatedCoordinate[1] = system[i].position.getX();
		duplicatedCoordinate[2] = system[i].position.getY();
		duplicatedCoordinate[3] = system[i].position.getY();

		for (size_t j = 0; j < 4; j++) {
			system[i].deltaWall[j] = system[i].radius - pow(-1, j) * (duplicatedCoordinate[j] - border[j]);
			if (system[i].deltaWall[j] < 0) {
				system[i].deltaWall[j] = 0;
			}
		}
	}

}
#include "physics.h"

// force function is considered not to be dependent on time explicitly
void Verlet(Particle& particle, const std::vector<Particle>& system, double timestep, const double border[4]) {
	/*Vector force = applyForce(particle, system, border);

	//position on the next step
	particle.position.setX(particle.position.getX() +
		particle.velocity.getX() * timestep +
		(force.getX() / 2 / particle.mass) * timestep * timestep);
	particle.position.setY(particle.position.getY() +
		particle.velocity.getY() * timestep +
		(force.getY() / 2 / particle.mass) * timestep * timestep);

	//intermediate velocity
	particle.velocity.setX(particle.velocity.getX() +
		timestep * force.getX() / 2 / particle.mass);
	particle.velocity.setY(particle.velocity.getY() +
		timestep * force.getY() / 2 / particle.mass);

	//velocity on the next step
	force = applyForce(particle, system, border); //updating the force vector
	particle.velocity.setX(particle.velocity.getX() +
		timestep * force.getX() / 2 / particle.mass);
	particle.velocity.setY(particle.velocity.getY() +
		timestep * force.getY() / 2 / particle.mass);*/

	Vector forceOld = applyForce(particle, system, border);
	Vector forceNew;

	particle.position.setX(particle.position.getX() +
		particle.velocity.getX() * timestep +
		(forceOld.getX() / (2 * particle.mass)) * timestep * timestep);
	particle.position.setY(particle.position.getY() +
		particle.velocity.getY() * timestep +
		(forceOld.getY() / (2 * particle.mass)) * timestep * timestep);

	forceNew = applyForce(particle, system, border);

	particle.velocity.setX(particle.velocity.getX() + timestep / (2 * particle.mass) * (forceOld.getX() + forceNew.getX()));
	particle.velocity.setY(particle.velocity.getY() + timestep / (2 * particle.mass) * (forceOld.getY() + forceNew.getY()));
}

Vector applyForce(const Particle& particle, const std::vector<Particle>& system, const double border[4]) {
	Vector resultant = Vector();

	resultant += applyWeight(particle);
	resultant += applyRepulsion(particle, system);
	resultant += applyWallRepulsion(particle, border);

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
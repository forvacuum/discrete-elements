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

	refreshDeltaWall(grid, border);

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
	resultant += applyRepulsion(particle, grid);
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

						if (delta < 0 || *it == &particle) {
							it++;
							continue;
						}

						result += Particle::stiffness * delta * delta / 4;

						it++;
					}
				}
			}
		}
	}

	//size_t size = particle.delta.size();

	//for (size_t i = 0; i < size; i++) {
	//	result += Particle::stiffness * particle.delta[i] * particle.delta[i] / 4;
	//}

	return result;
}

double elasticWallEnergy(const Particle& particle) {
	double result = 0;

	for (int i = 0; i < 4; i++) {
		result += Particle::stiffness * particle.deltaWall[i] * particle.deltaWall[i] / 2;
	}

	return result;
}

void refreshDeltaWall(std::vector<GridCell>& grid, const double border[4]) {
	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator it_end;

	/* Left wall */
	for (int i = 0; i < GridCell::verticalAmount; i++) {
		it = grid.at(i).contents.begin(); //going through particles in all left cells

		//ending marker
		it_end = grid.at(i).contents.end();

		while (it != it_end) {
			(*it)->deltaWall[0] = (*it)->radius - ((*it)->position.getX() - border[0]);
			if ((*it)->deltaWall[0] < 0) {
				(*it)->deltaWall[0] = 0;
			}

			it++;
		}
	}

	/* Right wall */
	for (int i = 0; i < GridCell::verticalAmount; i++) {
		size_t tmp = i + (GridCell::horizontalAmount - 1) * GridCell::verticalAmount;
		it = grid.at(i + (GridCell::horizontalAmount - 1) * GridCell::verticalAmount).contents.begin(); //going through particles in all right cells

		//ending marker
		it_end = grid.at(i + (GridCell::horizontalAmount - 1) * GridCell::verticalAmount).contents.end();

		while (it != it_end) {
			(*it)->deltaWall[1] = (*it)->radius + ((*it)->position.getX() - border[1]);
			if ((*it)->deltaWall[1] < 0) {
				(*it)->deltaWall[1] = 0;
			}

			it++;
		}
	}

	/* Bottom wall */
	for (int i = 0; i < GridCell::horizontalAmount; i++) {
		it = grid.at(i * GridCell::verticalAmount).contents.begin(); //going through particles in all bottom cells

		//ending marker
		it_end = grid.at(i * GridCell::verticalAmount).contents.end();

		while (it != it_end) {
			(*it)->deltaWall[2] = (*it)->radius - ((*it)->position.getY() - border[2]);
			if ((*it)->deltaWall[2] < 0) {
				(*it)->deltaWall[2] = 0;
			}

			it++;
		}
	}

	/* Top wall */
	for (int i = 0; i < GridCell::horizontalAmount; i++) {
		it = grid.at(i * GridCell::verticalAmount + GridCell::verticalAmount - 1).contents.begin(); //going through particles in all top cells

		//ending marker
		it_end = grid.at(i * GridCell::verticalAmount + GridCell::verticalAmount - 1).contents.end();

		while (it != it_end) {
			(*it)->deltaWall[3] = (*it)->radius + ((*it)->position.getY() - border[3]);
			if ((*it)->deltaWall[3] < 0) {
				(*it)->deltaWall[3] = 0;
			}

			it++;
		}
	}


	//size_t size;
	//double duplicatedCoordinate[4];
	//Vector relativePosition;

	//for (size_t i = 0; i < size; i++) {
	//	system[i].delta[i] = 0;
	//	for (size_t j = 0; j < i; j++) {
	//		relativePosition = system[i].position - system[j].position;
	//		system[i].delta[j] = system[i].radius + system[j].radius - Vector::norm(relativePosition);
	//		if (system[i].delta[j] < 0) {
	//			system[i].delta[j] = 0;
	//		}
	//		system[j].delta[i] = system[i].delta[j];
	//	}
	//}

	//for (size_t i = 0; i < size; i++) {
	//	duplicatedCoordinate[0] = system[i].position.getX();
	//	duplicatedCoordinate[1] = system[i].position.getX();
	//	duplicatedCoordinate[2] = system[i].position.getY();
	//	duplicatedCoordinate[3] = system[i].position.getY();

	//	for (size_t j = 0; j < 4; j++) {
	//		system[i].deltaWall[j] = system[i].radius - pow(-1, j) * (duplicatedCoordinate[j] - border[j]);
	//		if (system[i].deltaWall[j] < 0) {
	//			system[i].deltaWall[j] = 0;
	//		}
	//	}
	//}
}
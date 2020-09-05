#include "particle.h"

double Particle::maxRadius = 0;
double Particle::stiffness = 0;

Particle::Particle() {
	position = Vector();
	velocity = Vector();
	gridRow = 0;
	gridColumn = 0;

	radius = 0;
	mass = 0;

	for (int i = 0; i < 4; i++) {
		deltaWall[i] = 0;
	}
}

bool Particle::operator==(const Particle& p) const {
	//return this == &p;
	return this->position == p.position
		&& this->velocity == p.velocity
		&& this->mass == p.mass
		&& this->radius == p.radius;
}

bool Particle::operator!=(const Particle& p) const {
	//return this != &p;
	return this->position != p.position
		|| this->velocity != p.velocity
		|| this->mass != p.mass
		|| this->radius != p.radius;
}

void Particle::refreshGridCoordinates(std::vector<GridCell>& grid) {
	double oldCellX = gridRow * GridCell::length;
	double oldCellY = gridColumn * GridCell::width;

	size_t oldCellIndex = gridRow * GridCell::verticalAmount + gridColumn;
	size_t newCellIndex;

	if (position.getX() >= oldCellX + GridCell::length 
			&& gridRow < GridCell::horizontalAmount - 1) {
		gridRow++;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * GridCell::verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);

	}
	else if (position.getX() < oldCellX && oldCellX > 0) {
		gridRow--;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * GridCell::verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);
	}

	if (position.getY() >= oldCellY + GridCell::width 
			&& gridColumn < GridCell::verticalAmount - 1) {
		gridColumn++;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * GridCell::verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);
	} 
	else if (position.getY() < oldCellY && oldCellY > 0) {
		gridColumn--;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * GridCell::verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);
	}
}

void Particle::setGridCellPositions(std::vector<Particle>& system, const std::vector<GridCell>& grid) {
	auto it = system.begin();
	int i = 0;
	int j = 0;

	while (it != system.end()) {
		while (i * GridCell::length < it->position.getX()) {
			i++;
		}
		i--;
		
		while (j * GridCell::width < it->position.getY()) {
			j++;
		}
		j--;

		//it->gridCellIndex = i * GridCell::verticalAmount + j;
		it->gridRow = i;
		it->gridColumn = j;
		it++;
	}
}
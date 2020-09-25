#include "particle.h"

double Particle::maxRadius = 0;
double Particle::stiffness = 0;
bool Particle::isWallEnabled[4] = {true, true, true, true};

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

void Particle::refreshDeltaWall(std::vector<GridCell>& grid, const double border[4]) {
	std::vector<Particle*>::iterator it;
	std::vector<Particle*>::iterator it_end;

	/* Left wall */
	if (isWallEnabled[0]) {
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
	}

	/* Right wall */
	if (isWallEnabled[1]) {
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
	}

	/* Bottom wall */
	if (isWallEnabled[2]) {
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
	}

	/* Top wall */
	if (isWallEnabled[3]) {
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
	}
}
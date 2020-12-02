#include "particle.h"
#include "grid.h"

double Particle::minRadius = 0;
double Particle::maxRadius = 0;
double Particle::stiffnessRepulsive = 0;
double Particle::stiffnessAttractive = 0;
double Particle::stiffnessShear = 0;
double Particle::particleFriction = 0;
double Particle::wallFriction = 0;
double Particle::floorFriction = 0;
double Particle::criticalDistance = 0;
bool Particle::isWallEnabled[4] = {true, true, true, true};
bool Particle::isPacked = false;

Particle::Particle() {
	position = Vector();
	velocity = Vector();
	gridRow = 0;
	gridColumn = 0;

	radius = 0;
	mass = 0;

	for (double & i : deltaWall) {
		i = 0;
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

void Particle::refreshGridCoordinates(Grid& grid, const double border[4]) {
	double oldCellX = grid.workspaceBorder[0] + gridRow * GridCell::width;
	double oldCellY = grid.workspaceBorder[2] + gridColumn * GridCell::height;

	size_t oldCellIndex = gridRow * grid.verticalAmount + gridColumn;
	size_t newCellIndex;

//	if(position.getX() <= border[0] || position.getX() >= border[1] ||
//            position.getY() <= border[2] || position.getY() >= border[3]) {
//	    return;
//	}
	for (double d : deltaWall) {
        if(d > 0) {
            return;
        }
	}

	if (position.getX() >= oldCellX + GridCell::width
			&& gridRow < grid.horizontalAmount - 1) {
		gridRow++;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * grid.verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);

	}

	else if (position.getX() < oldCellX && oldCellX != 0) {
		gridRow--;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * grid.verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);
	}

	if (position.getY() >= oldCellY + GridCell::height
			&& gridColumn < grid.verticalAmount - 1) {
		gridColumn++;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * grid.verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);
	}

	else if (position.getY() < oldCellY && oldCellY != 0) {
		gridColumn--;
		grid.at(oldCellIndex).removeParticle(this);
		newCellIndex = gridRow * grid.verticalAmount + gridColumn;
		grid.at(newCellIndex).addParticle(this);
	}
}

void Particle::setGridCellPositions(std::vector<Particle> &system, Grid& grid) {
    auto it = system.begin();
    int i = 0;
    int j = 0;

    while (it != system.end()) {
        while (grid.workspaceBorder[0] + i * GridCell::width < it->position.getX()) {
            i++;
        }
        i--;

        while (grid.workspaceBorder[2] + j * GridCell::height < it->position.getY()) {
            j++;
        }
        j--;

        it->gridRow = i;
        it->gridColumn = j;
        it++;
    }
}

//TODO: move the calculations of border coordinates somewhere else
//TODO: refactor this
void Particle::refreshDeltaWall(Grid& grid, const double border[4]) {
    // This method goes through all particles in the same cells as the border
    // and in the adjacent ones that are placed inside the container
	std::vector<Particle*>::iterator it1;
	std::vector<Particle*>::iterator it2;
	std::vector<Particle*>::iterator it1_end;
	std::vector<Particle*>::iterator it2_end;

	size_t borderCoordinateInGrid = 0;
	size_t currentCellNumber;

	/* Left wall */
    while (grid.workspaceBorder[0] + borderCoordinateInGrid * GridCell::width < border[0] - GridCell::width) {
        borderCoordinateInGrid++;
    }
	if (isWallEnabled[0]) {
		for (size_t i = 0; i < grid.verticalAmount; i++) {
		    currentCellNumber = i + borderCoordinateInGrid * grid.verticalAmount;

            it1 = grid.at(currentCellNumber).contents.begin();
            it1_end = grid.at(currentCellNumber).contents.end();

            currentCellNumber = i + (borderCoordinateInGrid + 1) * grid.verticalAmount;
            it2 = grid.at(currentCellNumber).contents.begin();
            it2_end = grid.at(currentCellNumber).contents.end();

			while (it1 != it1_end) {
				(*it1)->deltaWall[0] = (*it1)->radius - ((*it1)->position.getX() - border[0]);
				if ((*it1)->deltaWall[0] < 0) {
					(*it1)->deltaWall[0] = 0;
				}
				it1++;
			}
            while (it2 != it2_end) {
                (*it2)->deltaWall[0] = (*it2)->radius - ((*it2)->position.getX() - border[0]);
                if ((*it2)->deltaWall[0] < 0) {
                    (*it2)->deltaWall[0] = 0;
                }
                it2++;
            }
		}
	}

	/* Right wall */
    while (grid.workspaceBorder[0] + borderCoordinateInGrid * GridCell::width < border[1] - GridCell::width) {
        borderCoordinateInGrid++;
    }
    if (isWallEnabled[1]) {
		for (int i = 0; i < grid.verticalAmount; i++) {
            currentCellNumber = i + borderCoordinateInGrid * grid.verticalAmount;
			it1 = grid.at(currentCellNumber - 1).contents.begin();
            it1_end = grid.at(currentCellNumber - 1).contents.end();

            currentCellNumber = i + (borderCoordinateInGrid - 1) * grid.verticalAmount;
            it2 = grid.at(currentCellNumber).contents.begin();
            it2_end = grid.at(currentCellNumber).contents.end();

			while (it1 != it1_end) {
				(*it1)->deltaWall[1] = (*it1)->radius + ((*it1)->position.getX() - border[1]);
				if ((*it1)->deltaWall[1] < 0) {
					(*it1)->deltaWall[1] = 0;
				}
				it1++;
			}
            while (it2 != it2_end) {
                (*it2)->deltaWall[1] = (*it2)->radius + ((*it2)->position.getX() - border[1]);
                if ((*it2)->deltaWall[1] < 0) {
                    (*it2)->deltaWall[1] = 0;
                }

                it2++;
            }
		}
	}

	/* Bottom wall */
    borderCoordinateInGrid = 0;
    while (grid.workspaceBorder[2] + borderCoordinateInGrid * GridCell::height < border[2] - GridCell::height) {
        borderCoordinateInGrid++;
    }
	if (isWallEnabled[2]) {
		for (int i = 0; i < grid.horizontalAmount; i++) {
		    currentCellNumber = i * grid.verticalAmount + borderCoordinateInGrid;
            it1 = grid.at(currentCellNumber).contents.begin();
            it1_end = grid.at(currentCellNumber).contents.end();

            it2 = grid.at(currentCellNumber + 1).contents.begin();
            it2_end = grid.at(currentCellNumber + 1).contents.end();

			while (it1 != it1_end) {
				(*it1)->deltaWall[2] = (*it1)->radius - ((*it1)->position.getY() - border[2]);
				if ((*it1)->deltaWall[2] < 0) {
					(*it1)->deltaWall[2] = 0;
				}
				it1++;
			}
            while (it2 != it2_end) {
                (*it2)->deltaWall[2] = (*it2)->radius - ((*it2)->position.getY() - border[2]);
                if ((*it2)->deltaWall[2] < 0) {
                    (*it2)->deltaWall[2] = 0;
                }
                it2++;
            }
		}
	}

	borderCoordinateInGrid = 0;
	/* Top wall */
    while (grid.workspaceBorder[2] + borderCoordinateInGrid * GridCell::height < border[3] - GridCell::height) {
        borderCoordinateInGrid++;
    }
    if (borderCoordinateInGrid == grid.verticalAmount) { //crutch?
        borderCoordinateInGrid--;
    }
	if (isWallEnabled[3]) {
		for (int i = 0; i < grid.horizontalAmount; i++) {
            currentCellNumber = i * grid.verticalAmount + borderCoordinateInGrid;
            it1 = grid.at(currentCellNumber).contents.begin();
			it1_end = grid.at(currentCellNumber).contents.end();

            it2 = grid.at(currentCellNumber - 1).contents.begin();
            it2_end = grid.at(currentCellNumber - 1).contents.end();

			while (it1 != it1_end) {
				(*it1)->deltaWall[3] = (*it1)->radius + ((*it1)->position.getY() - border[3]);
				if ((*it1)->deltaWall[3] < 0) {
					(*it1)->deltaWall[3] = 0;
				}
				it1++;
			}
            while (it2 != it2_end) {
                (*it2)->deltaWall[3] = (*it2)->radius + ((*it2)->position.getY() - border[3]);
                if ((*it2)->deltaWall[3] < 0) {
                    (*it2)->deltaWall[3] = 0;
                }
                it2++;
            }
		}
	}
}

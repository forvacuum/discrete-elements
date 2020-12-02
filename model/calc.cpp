#include "calc.h"
#include "impexp.h"
#include "force.h"

/* This function creates a file named as specified in the argument 'particlesFilename'
	containing initial data for given amount of randomly generated particles

	File structure:
	a(1)	/rho(1)	x_0(1)	y_0(1)	v_x(1)	v_y(1)
	...		...		...		...		...		...
	a(N)	/rho(N)	x_0(N)	y_0(N)	v_x(N)	v_y(N)		 */
void generateParticlesTriangle(const std::string& infoFilename, const std::string& particlesFilename, const double border[4]) {
	srand(time(0));

	size_t amount;
	size_t N = 0;
	size_t rowCounter = 0;

	double minRadius;
	double maxRadius;
	double currentRadius;
	double tmp;

	double density;

	double currentX;
	double currentY;
	double horizontalStep;
	double verticalStep;

	std::ifstream fin(infoFilename);
	fin >> amount >> minRadius >> maxRadius >> density;
	fin.close();

	currentY = border[2] + maxRadius;
	horizontalStep = 2 * maxRadius;
	verticalStep = sqrt(3) * maxRadius;

	std::ofstream fout(particlesFilename);
	while (N < amount) {

		if (rowCounter % 2 == 0) {
			currentX = border[0] + maxRadius;
		}
		else {
			currentX = border[0] + 2 * maxRadius;
		}

		while (currentX < border[1] - maxRadius) {
			tmp = (double)rand() / RAND_MAX; //random real number in the interval [0, 1)
			currentRadius = minRadius + tmp * (maxRadius - minRadius);
			fout << currentRadius << " " << density << " " << currentX << " " << currentY << " 0 0" << std::endl;
			N++;
			if (N >= amount) break;
			currentX += horizontalStep;
		}

		rowCounter++;
		currentY += verticalStep;
	}

	fout.close();
}

double pack(std::ofstream& fout, std::ofstream& fout_e, std::vector<Particle>& system,
          Grid& grid, double timeStep, const double border[4]) {
    double packTime = 0;
    double energyDiff;
    double systemEnergy = 0;
    double eps = 1e-4;
    do {
        fout << packTime << " ";
        appendSystemPosition(fout, system);
        appendSystemEnergy(fout_e, system, grid, border);
        calculateNextIteration(system, grid, timeStep, border);
        packTime += timeStep;

    } while (abs(calculateEnergyRelation(system, border)) >= eps || packTime < 0.1);

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
                    if (j >= 0 && j < grid.verticalAmount) { // checking for column index correctness
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

double removeWall(std::ofstream& fout, std::ofstream& fout_e, std::vector<Particle>& system,
                  Grid& grid, double timeStep, double packTime, const double *border) {
    double workTime = 0;
    double totalTime = packTime;
    double eps = 1e-6;
    Particle::isWallEnabled[1] = false;

    do {
        //fout << totalTime << " ";
        //appendSystemPosition(fout, system);
        //appendSystemEnergy(fout_e, system, grid, border);
        calculateNextIteration(system, grid, timeStep, border);
        workTime += timeStep;
        totalTime += timeStep;

    } while (abs(calculateEnergyRelation(system, border)) >= eps || workTime < 0.1);

    return workTime;
}

double shiftWall(std::ofstream& fout, std::ofstream& fout_e, std::vector<Particle>& system,
                  Grid& grid, double timeStep, double packTime, double *border) {
    double workTime = 0;
    double totalTime = packTime;
    double eps = 1e-6;
    //border[3] = highestOrdinate(system) + Particle::maxRadius;

    do {
        fout << totalTime << " ";
        appendSystemPosition(fout, system);
        appendSystemEnergy(fout_e, system, grid, border);
        if (border[2] >= highestOrdinate(system) / 10) {
            break;
        }
        border[2] += 1e-5;
        calculateNextIteration(system, grid, timeStep, border);
        workTime += timeStep;
        totalTime += timeStep;

    } while (true);

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

double highestOrdinate(const std::vector<Particle>& system) {
    auto it = system.begin();
    double maxY = system.begin()->position.getY();

    while (it != system.end()) {
        if(it->position.getY() > maxY) {
            maxY = it->position.getY();
        }
        it++;
    }

    return maxY;
}
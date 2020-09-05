#include "grid.h"

double GridCell::defaultSize = 0;
double GridCell::length = 0;
double GridCell::width = 0;
size_t GridCell::horizontalAmount = 0;
size_t GridCell::verticalAmount = 0;

GridCell::GridCell() {
	x = 0;
	y = 0;
}

GridCell::GridCell(double x, double y) {
	this->x = x;
	this->y = y;
}


void GridCell::addParticle(Particle* p) {
	if (std::find(contents.begin(), contents.end(), p) == contents.end()) {
		contents.push_back(p);
	}
}

void GridCell::removeParticle(Particle* p) {
	auto it = std::find(contents.begin(), contents.end(), p);
	if (it != contents.end()) {
		contents.erase(it);
	}
}

std::vector<GridCell> GridCell::setGrid(double border[4]) {
	double totalLength = border[1] - border[0];
	double totalWidth = border[3] - border[2];
	length = getPreferredSize(totalLength);
	width = getPreferredSize(totalWidth);
	GridCell cell;
	size_t i = 0;
	size_t j = 0;
	std::vector<GridCell> grid;

	while (i * length < totalLength) { // might give an error because of floating point???
		j = 0;
		cell.x = i * length;
		while (j * width < totalWidth) {
			cell.y = j * width;
			grid.push_back(cell);
			j++;
		}
		i++;
	}
	horizontalAmount = i;
	verticalAmount = j;

	return grid;
}

double GridCell::getPreferredSize(double workspaceMeasure) {
	double m = floor(workspaceMeasure / defaultSize); // should be checked out // amount of cells sized '2 * maxRadius' that can be put into the current workspace
	double residual = workspaceMeasure - defaultSize * m;
	double additionalSize = residual / m;
	return defaultSize + additionalSize;
}

void GridCell::setCellsContents(std::vector<GridCell>& grid, std::vector<Particle>& system) {
	auto it = system.begin();
	size_t cellIndex;
	while (it != system.end()) {
		cellIndex = it->gridRow * GridCell::verticalAmount + it->gridColumn;
		grid.at(cellIndex).contents.push_back(&(*it));
		it++;
	}
}

#include "particle.h"
#include "grid.h"

double GridCell::defaultSize = 0;
double GridCell::width = 0;
double GridCell::height = 0;

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

double GridCell::getPreferredSize(double workspaceSize) { // horizontal or vertical size
    // amount of cells sized '2 * maxRadius' that can be put into the current workspace
	double m = floor(workspaceSize / defaultSize);
	double residual = workspaceSize - defaultSize * m;
	double additionalSize = residual / m;
	return defaultSize + additionalSize;
}

Grid::Grid(double* workspace) {
    workspaceBorder = workspace;
    grid = setGrid(workspaceBorder);
}

GridCell &Grid::at(size_t index) {
    return grid.at(index);
}

void Grid::setCellsContents(std::vector<Particle>& system) {
    auto it = system.begin();
    size_t cellIndex;
    while (it != system.end()) {
        cellIndex = it->gridRow * this->verticalAmount + it->gridColumn;
        this->at(cellIndex).contents.push_back(&(*it));
        it++;
    }
}

std::vector<GridCell> Grid::setGrid(const double workspace[4]) {
    std::vector<GridCell> result;

    double totalWidth = workspace[1] - workspace[0];
    double totalHeight = workspace[3] - workspace[2];
    GridCell::width = GridCell::getPreferredSize(totalWidth);
    GridCell::height = GridCell::getPreferredSize(totalHeight);
    GridCell cell;
    size_t i = 0;
    size_t j = 0;

    while (workspace[0] + i * GridCell::width < workspace[1]) {
        j = 0;
        cell.x = workspace[0] + i * GridCell::width;
        while (workspace[2] + j * GridCell::height < workspace[3]) {
            cell.y = workspace[2] + j * GridCell::height;
            result.push_back(cell);
            j++;
        }
        i++;
    }
    horizontalAmount = i;
    verticalAmount = j;

    return result;
}

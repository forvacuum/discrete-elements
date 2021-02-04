#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>
#include <unordered_set>
#include <queue>

#include "exception.h"
#include "vector2D.h"
#include "particle.h"
#include "energy.h"


void generateParticlesTriangle(const std::string& infoFilename, const std::string& perticlesFilename, const double border[4]);

double pack(std::ofstream& fout, std::ofstream& fout_e, std::vector<Particle>& system, Grid* grid,
            double timeStep, const double border[4]);

void setNeighbours(std::vector<Particle>& system, Grid* grid);

double removeWall(std::ofstream &fout, std::ofstream &fout_e, std::vector<Particle> &system, Grid* grid,
                  double timeStep, const double border[4]);

double shiftWall(std::ofstream &fout, std::ofstream &fout_e, std::vector<Particle> &system, Grid* grid,
                  double timeStep, double border[4]);

void calculatePosition(std::vector<Particle>& system, Grid* grid, double timeStep, const double border[4]);

void calculateVelocity(std::vector<Particle>& system, Grid* grid, double timeStep, const double border[4]);

void calculateNextIteration(std::vector<Particle>& system, Grid* grid, double timeStep, const double border[4]);

double calculateEnergyRelation(const std::vector<Particle>& system, const double border[4]);

void checkBorderCrossed(std::vector<Particle>& system, const Grid* grid);

double highestOrdinate(const std::vector<Particle>& system);

Particle* farRight(std::vector<Particle>& system);

std::unordered_set<size_t> getEdge(std::vector<Particle>& system, Grid* grid);
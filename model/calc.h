#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>
#include <unordered_set>
#include <queue>

#include "vector2D.h"
#include "particle.h"
#include "energy.h"

void generateParticlesTriangle(const std::string&, const std::string&, const double[4]);

double pack(std::ofstream&, std::ofstream&, std::vector<Particle>&, Grid& , double, const double[4]);

void setNeighbours(std::vector<Particle>&, Grid&);

double removeWall(std::ofstream &fout, std::ofstream &fout_e, std::vector<Particle> &system, Grid &grid,
                  double timeStep, double packTime, const double border[4]);

double shiftWall(std::ofstream &fout, std::ofstream &fout_e, std::vector<Particle> &system, Grid &grid,
                  double timeStep, double packTime, double border[4]);

void calculatePosition(std::vector<Particle>&, Grid&, double, const double[4]);

void calculateVelocity(std::vector<Particle>&, Grid&, double, const double[4]);

void calculateNextIteration(std::vector<Particle>&, Grid&, double, const double[4]);

double calculateEnergyRelation(const std::vector<Particle>&, const double[4]);

double highestOrdinate(const std::vector<Particle>&);

Particle* farRight(std::vector<Particle>&);

std::unordered_set<size_t> getEdge(std::vector<Particle>&, Grid&);
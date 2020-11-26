#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>
#include "vector2D.h"
#include "particle.h"
#include "energy.h"

void generateParticlesTriangle(const std::string&, const std::string&, const double[4]);

double pack(std::ofstream&, std::ofstream&, std::vector<Particle>&, Grid& , double, const double[4]);

void setNeighbours(std::vector<Particle>&, Grid&);

double execute(std::ofstream&, std::ofstream&, std::vector<Particle>&, Grid&,
               double, double, const double[4]);

void calculatePosition(std::vector<Particle>&, Grid&, double, const double[4]);

void calculateVelocity(std::vector<Particle>&, Grid&, double, const double[4]);

void calculateNextIteration(std::vector<Particle>&, Grid&, double, const double[4]);

double calculateEnergyRelation(const std::vector<Particle>&, const double[4]);
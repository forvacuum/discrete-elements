#pragma once
#include <vector>

#include "particle.h"
#include "force.h"
#include "grid.h"

double calculateTotalEnergy(const Particle& particle, const double border[4], Grid* grid);
double calculateTotalSystemEnergy(const std::vector<Particle>& particle, const double border[4], Grid* grid);

double kineticEnergy(const Particle& particle);
double potentialEnergy(const Particle& particle, const double border[4]);
double normalForceEnergy(const Particle& particle, Grid* grid);
//double shearForceEnergy(const Particle& particle);
double elasticWallEnergy(const Particle& particle);
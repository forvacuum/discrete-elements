#pragma once
#include <vector>

#include "particle.h"
#include "force.h"
#include "grid.h"

double calculateTotalEnergy(const Particle&, const double[4], Grid&);
double calculateTotalSystemEnergy(const std::vector<Particle>&, const double[4], Grid&);

double kineticEnergy(const Particle&);
double potentialEnergy(const Particle&, const double[4]);
double normalForceEnergy(const Particle&, Grid&);
//double shearForceEnergy(const Particle&);
double elasticWallEnergy(const Particle&);
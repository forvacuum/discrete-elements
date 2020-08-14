#pragma once
#include <vector>

#include "particle.h"
#include "force.h"

void Verlet(Particle&, const std::vector<Particle>&, double, const double[4]);
Vector applyForce(const Particle&, const std::vector<Particle>&, const double[4]);
double kineticEnergy(const Particle&);
double potentialEnergy(const Particle&, const double[4]);
double elasticParticleEnergy(const Particle&, const std::vector<Particle>&);
double elasticWallEnergy(const Particle&, const double[4]);
double calculateTotalEnergy(const Particle&, const std::vector<Particle>&, const double[4]);
#pragma once
#include <vector>

#include "particle.h"
#include "force.h"

void calculatePosition(std::vector<Particle>&, double, const double[4]);
void calculateVelocity(std::vector<Particle>&, double, const double[4]);
void calculateNextIteration(std::vector<Particle>&, double, const double[4]);

Vector applyForce(const Particle&, const std::vector<Particle>&, const double[4]);

double kineticEnergy(const Particle&);
double potentialEnergy(const Particle&, const double[4]);

double elasticParticleEnergy(const Particle&);
double elasticWallEnergy(const Particle&);
double calculateTotalEnergy(const Particle&, const double[4]);

void refreshDelta(std::vector<Particle>&, const double[4]);
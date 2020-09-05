#pragma once
#include <vector>

#include "particle.h"
#include "force.h"
#include "grid.h"

void calculatePosition(std::vector<Particle>&, std::vector<GridCell>& , double, const double[4]);
void calculateVelocity(std::vector<Particle>&, std::vector<GridCell>&, double, const double[4]);
void calculateNextIteration(std::vector<Particle>&, std::vector<GridCell>&, double, const double[4]);

Vector applyForce(const Particle&, std::vector<GridCell>&, const double[4]);

double calculateTotalEnergy(const Particle&, const double[4], std::vector<GridCell>&);

double kineticEnergy(const Particle&);
double potentialEnergy(const Particle&, const double[4]);
double elasticParticleEnergy(const Particle&, std::vector<GridCell>&);
double elasticWallEnergy(const Particle&);

void refreshDeltaWall(std::vector<GridCell>&, const double[4]);
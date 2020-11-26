#pragma once
#include <vector>

#include "particle.h"
#include "force.h"
#include "grid.h"

double pack(std::ofstream&, std::ofstream&, std::vector<Particle>&, Grid& , double, const double[4]);
void setNeighbours(std::vector<Particle>&, Grid&);
double execute(std::ofstream&, std::ofstream&, std::vector<Particle>&, Grid&,
        double, double, const double[4]);

void calculatePosition(std::vector<Particle>&, Grid&, double, const double[4]);
void calculateVelocity(std::vector<Particle>&, Grid&, double, const double[4]);
void calculateNextIteration(std::vector<Particle>&, Grid&, double, const double[4]);

Vector applyForce(Particle&, Grid&, const double[4], double);

double calculateEnergyRelation(const std::vector<Particle>&, const double[4]);
double calculateTotalEnergy(const Particle&, const double[4], Grid&);
double calculateTotalSystemEnergy(const std::vector<Particle>&, const double[4], Grid&);

double kineticEnergy(const Particle&);
double potentialEnergy(const Particle&, const double[4]);
double normalForceEnergy(const Particle&, Grid&);
//double shearForceEnergy(const Particle&);
double elasticWallEnergy(const Particle&);
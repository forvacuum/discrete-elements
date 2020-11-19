#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>

#include "vector2D.h"
#include "particle.h"
#include "physics.h"

void initialize(double*, double&, bool&, bool&, bool&);
//void generateParticlesRandom(const std::string&, const std::string&, double[4]);
void generateParticlesTriangle(const std::string&, const std::string&, const double[4]);
std::vector <Particle> importParticles(const std::string &sourceFile, const std::string &constantsFile);
void exportParticles(const std::string&, const std::vector<Particle>&);
void exportDetails(const std::string&, const double[4], const std::vector<Particle>&);
void appendSystemPosition(std::ofstream&, const std::vector<Particle>&);
double appendSystemEnergy(std::ofstream&, const std::vector<Particle>&, std::vector<GridCell>&, const double[4]);
double totalPositionNorm(const std::vector<Particle> &system);
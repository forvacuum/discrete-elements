#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>

#include "vector2D.h"
#include "particle.h"
#include "energy.h"

constexpr auto pi = 3.1415926535;

std::vector <Particle> importParticles(const std::string &sourceFile, const std::string &constantsFile);
void exportParticles(const std::string&, const std::vector<Particle>&);
void exportDetails(const std::string&, const double[4], const std::vector<Particle>&);
void appendSystemPosition(std::ofstream&, const std::vector<Particle>&);
double appendSystemEnergy(std::ofstream&, const std::vector<Particle>&, Grid&, const double[4]);
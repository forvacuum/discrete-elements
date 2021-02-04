#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <iterator>
#include <unordered_set>

#include "vector2D.h"
#include "particle.h"
#include "energy.h"

constexpr auto pi = 3.1415926535;

std::vector <Particle> importParticles(const std::string& sourceFile, const std::string& constantsFile);
void exportParticles(const std::string& filename, const std::vector<Particle>& system);
void exportDetails(const std::string& filename, const double border[4], const std::vector<Particle>& system);
void exportEdge(const std::string& outputFilename, Grid* grid, std::unordered_set<size_t> edge);
void exportGrid(const std::string& outputFilename, Grid* grid);
void appendSystemPosition(std::ofstream& fout, const std::vector<Particle>& system);
double appendSystemEnergy(std::ofstream& fout, const std::vector<Particle>& system, Grid* grid, const double border[4]);
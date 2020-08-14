#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

#include "vector2D.h"
#include "particle.h"

void generateParticles(std::string, std::string, double[4]);
std::vector<Particle> importParticles(std::string, double);
void exportDetails(std::string, const double[4], const std::vector<Particle>);
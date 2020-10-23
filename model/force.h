#pragma once

#include <vector>
#include <iterator>
#include <unordered_map>

#include "vector2D.h"
#include "particle.h"
#include "grid.h"

constexpr auto g = 9.80665;
constexpr auto dampingCoefficient = 1;

Vector applyWeight(const Particle&);
Vector applyNormalForce(const Particle&, std::vector<GridCell>&);
Vector applyShearForce(Particle&, std::vector<GridCell>&, double timestep);
Vector applyWallRepulsion(const Particle&, const double[4]);
Vector applyDissipation(const Particle&);
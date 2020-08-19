#pragma once

#include <vector>

#include "vector2D.h"
#include "particle.h"

constexpr auto g = 9.80665;
constexpr auto dampingCoefficient = 0.5;

Vector applyWeight(const Particle&);
Vector applyRepulsion(const Particle&, const std::vector<Particle>&);
Vector applyWallRepulsion(const Particle&, const double[4]);
Vector applyDissipation(const Particle&);
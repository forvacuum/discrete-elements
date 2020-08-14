#pragma once

#include <vector>

#include "vector2D.h"
#include "particle.h"

constexpr auto g = 9.80665;

Vector applyWeight(const Particle&);
Vector applyRepulsion(const Particle&, const std::vector<Particle>&);
Vector applyWallRepulsion(const Particle&, const double[4]);
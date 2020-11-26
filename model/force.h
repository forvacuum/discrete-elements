#pragma once

#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <vector>

#include "vector2D.h"
#include "particle.h"
#include "grid.h"

constexpr auto g = 9.80665;
constexpr auto dampingCoefficient = 1;

struct Particle;
struct Grid;

Vector applyWeight(const Particle&);
Vector applyNormalForce(Particle&, Grid&);
Vector applyShearForce(Particle&, Grid&, double);
Vector applyWallRepulsion(const Particle&, const double[4]);
Vector applyWallFriction(const Particle&, const double[4]);
Vector applyDissipation(const Particle&);
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
Vector applyWallRepulsion(const Particle &p);
Vector applyWallFriction(const Particle &p);
Vector applyDissipation(const Particle&);

Vector applyForce(Particle&, Grid&, const double[4], double);
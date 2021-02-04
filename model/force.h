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

Vector applyWeight(const Particle& particle);
Vector applyNormalForce(Particle& particle, Grid* grid);
Vector applyShearForce(Particle& particle, Grid* grid, double timeStep);
Vector applyWallRepulsion(const Particle& particle);
Vector applyWallFriction(const Particle& particle);
Vector applyDissipation(const Particle& particle);

Vector applyForce(Particle& particle, Grid* grid, const double border[4], double timeStep);
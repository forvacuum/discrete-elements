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
double* setWorkspace(const double[]);
#include "particle.h"

bool Particle::operator==(const Particle& p) const {
	return this == &p;
}

bool Particle::operator!=(const Particle& p) const {
	return this != &p;
}
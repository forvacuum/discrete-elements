#pragma once
#include <stdexcept>

#include "particle.h"

class ParticleOutOfBorderException : public std::exception {
private:
    Particle* p;

public:
    ParticleOutOfBorderException(Particle* p);
    Particle* what();
};

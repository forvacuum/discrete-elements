#include "exception.h"

ParticleOutOfBorderException::ParticleOutOfBorderException(Particle* p) {
    this->p = p;
}

Particle* ParticleOutOfBorderException::what() {
    return p;
}
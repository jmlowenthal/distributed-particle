#pragma once

#include "Matrix.h"

#define SIZE 8
#define SEC_PER_UPDATE 10000.0f
#define G 6.6e-11

typedef float VariableType;

struct Particle {
	Vector<3, VariableType> p, v, a;

	void update(VariableType dt);
};

inline void Particle::update(VariableType dt) {
	v += a * dt;
	p += v * dt;

	a.at_(0) = 0;
	a.at_(1) = 0;
	a.at_(2) = 0;
}

void simulate(Particle * particles);
void update(Particle * particles);
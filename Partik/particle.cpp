#include "particle.h"
#include "_math.h"

#include <cmath>
#include <float.h>
#include <thread>

void update(Particle * particles) {
	for (unsigned int i = 0; i < SIZE; ++i) {
		Particle &p = particles[i];
		p.update(SEC_PER_UPDATE);
	}
}

#define PARALLELISE

void simulate(Particle * particles) {
#ifdef PARALLELISE

	static Matrix<SIZE, SIZE, Vector<3>> forces = Matrix<SIZE, SIZE, Vector<3>>();

#pragma loop(hint_parallel(0))
#pragma loop(ivdep)
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < i /*SIZE*/; ++j) {
			//if (i == j) continue;

			auto rel = particles[i].p - particles[j].p;
			double r = sq(rel.at_(0)) + sq(rel.at_(1)) + sq(rel.at_(2));

			if (r == 0.0) r = DBL_TRUE_MIN;
			//float invcrcub = float(power(r, -1.5));
			float invrcub = float(exp(-1.5 * log(r)));
			float coef = -(float)G * invrcub;

			Vector<3> & f = forces.at_(i, j);
			f.at_(0) = rel.at_(0) * coef;
			f.at_(1) = rel.at_(1) * coef;
			f.at_(2) = rel.at_(2) * coef;
		}
	}

#pragma loop(hint_parallel(0))
#pragma loop(ivdep)
	for (int i = 0; i < SIZE; ++i) {
		Particle &p = particles[i];
		for (int j = 0; j < SIZE; ++j) {
			if (i == j) continue;
			else if (j > i) {
				p.a -= forces.at_(j, i);
			}
			else {
				p.a += forces.at_(i, j);
			}
		}
	}
#else
	for (unsigned int i = 0; i < SIZE; ++i) {
		Particle &a = particles[i];
		for (unsigned int j = i + 1; j < SIZE; ++j) {
			Particle &b = particles[j];

			/*
			F = ma
			a = F/m
			r = rx * rx + ry * ry + rz * rz
			Let m = 1
			a = F
			F = GMm / (r^2) => a = G / (r^2)
			Fx = F * rx/r = G * rx / (r^3)
			*/

			Vector<3> rel = a.p - b.p;
			//auto r = (rel.transpose() * rel).at_(0); // <-- It's elegant but not efficent
			double r = sq(rel.at_(0)) + sq(rel.at_(1)) + sq(rel.at_(2));

			if (r == 0.0) r = DBL_TRUE_MIN;

			float invrcub = float(pow(r, -1.5));
			float coef = (float)G * invrcub;

			Vector<3> F = rel * coef;
			a.a -= F;
			b.a += F;
		}
	}
#endif
}
#include "rxParticle.h"

rxParticle::rxParticle(ofVec3f pos, float mass)
	:position(pos)
	,mass(mass)
	,velocity(0)
	,forces(0)
	,age(0)
	,lifetime(10)
{
	if(mass < 0.001) {
		mass = 0.001;
	}	
	inv_mass = 1.0/mass;
}

rxParticle::~rxParticle() {
	//printf("~rxParticle, lifetime: %f\n", lifetime);
}

void rxParticle::update() {
	forces *= inv_mass;
	velocity += forces;
	position += velocity;
	velocity *= 0.98f;
	forces.set(0,0,0);
}

void rxParticle::draw() {
	glVertex3fv(&position.x);
}

void rxParticle::reset() {
	neighbors.clear();
	//age = 0;
	lifetime = 10;
}
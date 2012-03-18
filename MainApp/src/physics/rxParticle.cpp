#include "rxParticle.h"

rxParticle::rxParticle(ofVec3f pos, float mass)
	:position(pos)
	,mass(mass)
	,velocity(0)
	,forces(0)
{
	if(mass < 0.001) {
		mass = 0.001;
	}	
	inv_mass = 1.0/mass;
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


#include "rxSwirp.h"
#include "rxParticles.h"
#include "rxSpring.h"


rxSwirp::rxSwirp(ofVec3f pos, rxParticles& particles, int numParticles) 
	:particles(particles)
{
	
	float mass = 1.0f;
	rxParticle* particle = NULL;
	rxParticle* prev_particle = NULL;
	rxSpring* spring = NULL;
	
	for(int i = 0; i < numParticles; ++i) {
		particle = new rxParticle(pos, mass);
		particles.addParticle(particle);
		if(prev_particle != NULL) {
			spring = new rxSpring(*particle, *prev_particle);
			particles.addSpring(spring);
		}
		prev_particle = particle;
		pos.y += 10;
	}
}

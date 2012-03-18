#ifndef ROXLU_RXPARTICLEH
#define ROXLU_RXPARTICLEH

#include "ofMain.h"

class rxParticle {
public:
	rxParticle(ofVec3f position, float mass);
	~rxParticle();

	void update();
	void draw();
	float mass;
	float inv_mass;
	ofVec3f forces;
	ofVec3f velocity;
	ofVec3f position;
};

#endif
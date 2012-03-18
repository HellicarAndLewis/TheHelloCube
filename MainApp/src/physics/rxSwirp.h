#ifndef ROXLU_RXSWIRPH
#define ROXLU_RXSWIRPH

#include "ofMain.h"
class rxParticles;
class rxSwirp {
public:
	rxSwirp(ofVec3f pos, rxParticles& p, int numParticles);
	rxParticles& particles;
};

#endif
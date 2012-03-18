#ifndef ROXLU_RXSPRINGH
#define ROXLU_RXSPRINGH
#include "ofMain.h"
class rxParticle;

class rxSpring {
public:
	rxSpring(rxParticle& a, rxParticle& b);
	~rxSpring();
	
	void update();
	void draw();	
	
	float length_sq;
	float k;
	float f;
	ofVec3f dir;
	rxParticle& a;
	rxParticle& b;
	float rest_length;
	float rest_length_sq;
	float length;
	int points;
};


#endif
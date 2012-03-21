#ifndef ROXLU_VEINSH
#define ROXLU_VEINSH

#include "ofMain.h"
#include "rxParticle.h"
#include "rxParticles.h"
#include <list>



class rxVeins {
public:
	rxVeins();
	~rxVeins();

	void addSource(rxParticle* s);
	void addRoot(rxParticle* r);
	void step();
	void update();
	void draw();
	
	rxParticles sources_ps;
	vector<rxParticle*> sources;
	vector<rxParticle*> roots;
	vector<ofVec3f> nodes;
	
	map<rxParticle*, list<rxParticle*> > sources_to_nodes;
};
#endif
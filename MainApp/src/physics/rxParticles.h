#ifndef ROXLU_RXPARTICLESH
#define ROXLU_RXPARTICLESH


#include "ofMain.h"
#include "rxParticle.h"
#include "rxSpring.h"
#include "rxSwirp.h"
#include <list>

using std::list;


class rxParticles {
public:
	rxParticles();
	~rxParticles();
	
	void draw();
	void update();
	void repel(float effect = 0.3);
	void follow(const ofVec3f& pos, float effect = 0.001f);
	void addForce(const ofVec3f& f);
	void addParticle(rxParticle* p);
	void addSpring(rxSpring* s);
	void addSwirp(rxSwirp* s);
	
	list<rxParticle*> particles;	
	list<rxSpring*> springs;
	list<rxSwirp*> swirps;
};

#endif
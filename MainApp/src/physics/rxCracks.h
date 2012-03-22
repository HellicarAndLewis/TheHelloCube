#ifndef ROXLU_CRACKSH
#define ROXLU_CRACKSH

#include "ofMain.h"
#include "AppAssets.h"
#include "rxParticle.h"
#include "rxParticles.h"
#include "ofxVoronoi.h"
#include "Error.h"

class rxCracks {	
public:
	rxCracks();
	~rxCracks();
	void setup(int w, int h);
	void generateRandomLine();
	void generateIsles(unsigned int num);
	void generateVoronoi();
	void generateCracks();
	void update();
	void draw();
	void repel(const float f);
	void centralize(const ofVec3f& c, float f = 0.001);
	void clear();
	ofxVoronoi voro;
	rxParticles particles;	
	rxParticles random_line;

	int w;
	int h;
	GLuint fbo;
	GLuint tex;
	GLuint depth;
};
#endif
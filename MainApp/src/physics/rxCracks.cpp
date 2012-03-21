#include "rxCracks.h"
rxCracks::rxCracks() 
{
}

rxCracks::~rxCracks() {
}

void rxCracks::generateRandomLine() {
	
}

void rxCracks::setup(int w, int h) {
	glGetError();
	w = w;
	h = h;
	
//	// fbo
//	glGenFramebuffers(1,&fbo); eglGetError();
//	glBindFramebuffer(GL_FRAMEBUFFER, fbo); eglGetError();
//	
//	
//	// tex
//	glGenTextures(1, &tex); eglGetError();
//	glActiveTexture(GL_TEXTURE0); eglGetError();
//	glBindTexture(GL_TEXTURE_2D, tex); eglGetError();
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); eglGetError();
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); eglGetError();
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); eglGetError();
//
//	// color & depth attachment.
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo, 0); eglGetError();
//	glGenRenderbuffers(1, &depth); eglGetError();
//	glBindRenderbuffer(GL_RENDERBUFFER, depth); eglGetError();	
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth); eglGetError();
//
//	GLenum drawbufs[] = {GL_COLOR_ATTACHMENT0};
//	glDrawBuffers(1, drawbufs);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void rxCracks::generateVoronoi() {
	list<rxParticle*>::iterator it = particles.begin();
	while(it != particles.end()) {
		rxParticle& p = *(*it);
		voro.addPoint(p.position.x, p.position.y);
		++it;
	}
	//voro.setMinDistance(15);
	voro.generateVoronoi();
}

void rxCracks::generateCracks() {
	for(int i  = 0; i <3; ++i) {
		generateIsles(100);
	}
	
	generateVoronoi();
	particles.repel(0.3);
	
	int cx = CUBE_SCREEN_WIDTH*0.5;
	int cy = CUBE_SCREEN_HEIGHT*0.5;
	ofVec3f c(ofRandom(cx-(cx*.2),cx+(cx*0.2)),ofRandom(cy-(cy*0.2),cy+(cy*0.2)) ,0);
	for(int i = 0; i < 70; ++i) {
		centralize(c,0.0008);
		particles.repel(0.3);
	}
	generateVoronoi();
	
	for(int i = 0; i < 100; ++i) {
		centralize(c,0.003);
		particles.repel(0.001);
	}
	generateVoronoi();
	
	for(int i = 0; i < 70; ++i) {
		centralize(c,0.0005);
		particles.repel(0.005);
	}
	generateVoronoi();
}

void rxCracks::generateIsles(unsigned int num) {
	int todo = num;
	ofVec3f p;
	rxParticle* particle;
	float radius = 15;
	int max_loops = 200;
	do { 
		p.set(ofRandom(0,CUBE_SCREEN_WIDTH), ofRandom(0,CUBE_SCREEN_HEIGHT),0);
		if(particles.isPositionFree(p, radius)) {
			particle = new rxParticle(p, 1);
			particles.addParticle(particle);
			--todo;
		}
		--max_loops;
	} while(todo > 0 && max_loops > 0);
}

void rxCracks::centralize(const ofVec3f& p, float f) {
	particles.follow(p,f);	
	particles.update();
}

void rxCracks::update() {
}

void rxCracks::draw() {
//	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//	glClearColor(0,0,0,1);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glViewport(0,0,w, h);
//

	
	vector<ofxVoronoiEdge>& edges = voro.edges;
	vector<ofxVoronoiEdge>::iterator eit = edges.begin();
	
	glColor3f(0.1,0.1,.1);
	float i = 0.3;
	int c = 0;
	while(eit != edges.end()) {
		ofxVoronoiEdge& e = *eit;
		if(c % 4 == 0) {
			glLineWidth(ofNoise(i)*7);
		}
		
		glBegin(GL_LINES);
			glVertex2fv(&e.a.x);
			glVertex2fv(&e.b.x);
		glEnd();
		i += 0.1;
		++eit;
		++c;
	}

//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glViewport(0,0,w, h);

	
}

void rxCracks::clear() {
	voro.clear();
	particles.clear();
}

void rxCracks::repel(const float f) {
}
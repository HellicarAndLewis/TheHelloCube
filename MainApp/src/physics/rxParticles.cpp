#include "rxParticles.h"


rxParticles::rxParticles() {
}

rxParticles::~rxParticles() {
}

void rxParticles::update() {
	list<rxParticle*>::iterator it = particles.begin();
	while(it != particles.end()) {
		(*it)->update();
		++it;
	}
	
	list<rxSpring*>::iterator its = springs.begin();
	while(its != springs.end()) {
		(*its)->update();
		++its;
	}
}

void rxParticles::draw() {
	glColor3f(0,0,0);
//	glBegin(GL_TRIANGLES);
	glBegin(GL_LINES);
		list<rxSpring*>::iterator its = springs.begin();
		while(its != springs.end()) {
			(*its)->draw();
			++its;
		}
	glEnd();
	
	glPointSize(8);
	glColor3f(0,0,0);
	glBegin(GL_POINTS);
		list<rxParticle*>::iterator it = particles.begin();
		while(it != particles.end()) {
			(*it)->draw();
			++it;
		}
	glEnd();
	
	
	glColor3f(1,1,1);
}

void rxParticles::addParticle(rxParticle* p) {
	particles.push_back(p);
}

void rxParticles::addSpring(rxSpring* s) {
	springs.push_back(s);
}

void rxParticles::addSwirp(rxSwirp* s) {
	swirps.push_back(s);
}

void rxParticles::addForce(const ofVec3f& f) {
	list<rxParticle*>::iterator it = particles.begin();
	while(it != particles.end()) {
		(*it)->forces += f;
		++it;
	}
}

void rxParticles::follow(const ofVec3f& f, float effect) {
	list<rxParticle*>::iterator it = particles.begin();
	ofVec3f dir;
	float force;
	float length;
	while(it != particles.end()) {
		rxParticle& p = *(*it);
		dir = f - p.position;
		length = dir.length();

		dir.normalize();
		dir *= length * effect;
		(*it)->forces += dir;
		++it;
	}
}

void rxParticles::repel(float effect) {
	ofVec3f dir;
	float f;
	float length_sq;
	
	int now = ofGetElapsedTimeMillis();
	list<rxParticle*>::iterator ita = particles.begin();
	list<rxParticle*>::iterator itb = particles.begin();
	while(ita != particles.end()) {	
		itb = ita;
		++itb;
		rxParticle& a = *(*ita);
		while(itb != particles.end()) {
			rxParticle& b = *(*itb);
			dir = b.position - a.position;
			length_sq = dir.lengthSquared();
			if(length_sq > 0.0f) {
				f = 1.0f/length_sq;
				dir *= (f * effect);
				a.forces -= dir;
				b.forces += dir;
			}
			++itb;
		}
		++ita;
	}
	int end = ofGetElapsedTimeMillis();
}


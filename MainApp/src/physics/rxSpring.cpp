#include "rxSpring.h"
#include "rxParticle.h"

#define USE_LINES

rxSpring::rxSpring(rxParticle& a, rxParticle& b)
	:a(a)
	,b(b)
	,k(0.1)
{
	rest_length_sq = (b.position-a.position).lengthSquared();
	rest_length = sqrtf(rest_length_sq);
	points = ofRandom(6,8);
}

rxSpring::~rxSpring() {
}

void rxSpring::update() {
	dir = b.position - a.position;
	length = dir.length();
	f = (rest_length -  length) * k;
	dir.normalize();
	dir *=f;
	a.position -= dir;
	b.position += dir;
}

void rxSpring::draw() {
//	glVertex3fv(&a.position.x);
//	glVertex3fv(&b.position.x);

	ofVec3f d = b.position - a.position;
	float len = d.length();
	d /= len;
	ofVec3f p(-d.y, d.x);
#ifdef USE_LINES
	p *= 5;
#else	
	p *= (len * 0.7);
#endif	
	
	float size = len/points;
	ofVec3f p1 = a.position;
	ofVec3f start;
	ofVec3f p2;
	ofVec3f p3;
	for(int i = 0; i < points; ++i) {
#ifndef USE_LINES
		p1 = start = a.position + (d*i*size);
		glVertex3fv(&p1.x);
		
		p2 = a.position + (d*(i+1)*size * 0.5);
		p1 = p2 - p;
		p3 = p2 + p;
		glVertex3fv(&p1.x);
		
		p2 = a.position + (d*(i+1)*size);
		glVertex3fv(&p2.x);		
		
		
		glVertex3fv(&p2.x);		
		glVertex3fv(&p3.x);		
		glVertex3fv(&start.x);		
#else
		p1 = start = a.position + (d*i*size);
		glVertex3fv(&start.x);
		p1 = start - p;
		glVertex3fv(&p1.x);
		
		glVertex3fv(&start.x);
		p1 = start + p;
		glVertex3fv(&p1.x);
		
#endif		
		//glVertex3fv(&p1.x);
	}
	
	

/*
	ofVec3f d = b.position - a.position;
	d *= 0.5;
	ofVec3f e = a.position + d;
	glColor3f(0,0,0); 
	glVertex3fv(&a.position.x);
	//glColor3f(0,1,0); glVertex3fv(&e.x);
	
	float x = d.x;
	d.x = -d.y;
	d.y = x;
	d *= 1.4;
	ofVec3f p = d +e;
	glVertex3fv(&p.x);
	
	glVertex3fv(&b.position.x);
	p = e + (d * -1.0);	
	glVertex3fv(&b.position.x);	
	glVertex3fv(&p.x);
	glVertex3fv(&a.position.x);
*/
}


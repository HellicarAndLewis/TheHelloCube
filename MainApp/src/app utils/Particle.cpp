
#include "Particle.h"

Particle::Particle() {
    uniquef = ofRandomuf();
    uniquei = ofRandom(-1000000, 1000000);
}
void Particle::draw() {
    ofCircle(pos, radius);
}

void Particle::setPosition(float x, float y) {
    pos.set(x, y);
}
void Particle::setPosition(const ofVec2f &p) {
    pos.set(p);
}

void Particle::wrapScreen(float x, float y, float width, float height) {
    
    if (pos.x < -radius) pos.x = width+radius;
    if (pos.y < -radius) pos.y = height+radius;
    if (pos.x > width+radius) pos.x = -radius;
    if (pos.y > height+radius) pos.y = -radius;
}

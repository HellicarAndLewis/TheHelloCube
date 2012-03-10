//
//  Particle.h
//  MainApp
//
//  Created by Todd Vanderlin on 3/9/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "Utils.h"

class Particle {

public:
    
    ofVec2f pos, vel, frc;
    float   radius;
    float   drag;
    float   uniquef;
    int     uniquei;
    
    Particle() {
        uniquef = ofRandomuf();
        uniquei = ofRandom(-1000000, 1000000);
    }
    void draw() {
        ofCircle(pos, radius);
    }
    
    void setPosition(float x, float y) {
        pos.set(x, y);
    }
    void setPosition(const ofVec2f &p) {
        pos.set(p);
    }
};
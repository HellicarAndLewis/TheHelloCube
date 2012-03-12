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
    
    Particle();
    void draw();
    
    void setPosition(float x, float y);
    void setPosition(const ofVec2f &p);
    
    void wrapScreen(float x=0, float y=0, float width=ofGetWidth(), float height=ofGetHeight());
};
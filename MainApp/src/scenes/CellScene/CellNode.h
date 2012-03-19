//
//  Cell.h
//  MainApp
//
//  Created by Todd Vanderlin on 3/9/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "Particle.h"
#include "ofMain.h"

class CellNode : public Particle {
  
public:
    
    bool bRemove;
    ofColor colour;
    
    CellNode() {
        bRemove = false;
        radius  = ofRandom(2,10);  
        drag    = ofRandom(0.97, 0.99);
        colour = ofColor(255,0,0);
    }
    
    void draw() {
        ofSetColor(colour);
        ofCircle(pos, radius);
    }
    
};

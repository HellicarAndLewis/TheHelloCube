//
//  Plant.h
//  emptyExample
//
//  Created by Todd Vanderlin on 2/22/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "Utils.h"
#include "Branch.h"

class Plant {
  
public:
    
    ofVec2f          p, v;
    ofVec2f          startPt;
    
    ofPolyline       pts;
    bool             bSplit;
    vector <Branch>  branches;
    // -------------------------------
    Plant();
    
    void grow(float x, float y);
    void update();
    void draw();
    
};


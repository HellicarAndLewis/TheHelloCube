//
//  SpotShape.h
//  MainApp
//
//  Created by Joel Lewis on 15/03/2012.
//  Copyright 2012 Hellicar&Lewis. All rights reserved.
//  Created by Todd Vanderlin on 3/2/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class SpotShape : public ofxBox2dCircle {
public:
    
    SpotShape();
    ~SpotShape();
    void update();
    
    float       growSpeed;
    float       radiusTarget;
};

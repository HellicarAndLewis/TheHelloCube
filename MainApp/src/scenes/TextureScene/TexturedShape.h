//
//  TexturedShape.h
//  MainApp
//
//  Created by Todd Vanderlin on 3/2/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class TexturedShape : public ofxBox2dCircle {
public:
    
    TexturedShape();
    ~TexturedShape();
    void update();
    
    float       growSpeed;
    float       radiusTarget;
    ofTexture * tex;
};
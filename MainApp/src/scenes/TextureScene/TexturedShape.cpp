//
//  TexturedShape.cpp
//  MainApp
//
//  Created by Todd Vanderlin on 3/2/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#include "TexturedShape.h"

TexturedShape::TexturedShape() {
    tex = NULL;
    growSpeed = ofRandom(0.03, 0.1);    
}

TexturedShape::~TexturedShape() {
    tex = NULL;
}

void TexturedShape::update() {
    ofxBox2dBaseShape::update();
    float r = getRadius();
    r += (radiusTarget - r)*growSpeed;
    setRadius(r);
}
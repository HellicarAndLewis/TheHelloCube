//
//  SpotShape.cpp
//  MainApp
//
//  Created by Joel Lewis on 15/03/2012.
//  Copyright 2012 Hellicar&Lewis. All rights reserved.
//

#include "SpotShape.h"

SpotShape::SpotShape() {
    growSpeed = ofRandom(0.03, 0.1);    
}

SpotShape::~SpotShape() {
}

void SpotShape::update() {
    ofxBox2dBaseShape::update();
    float r = getRadius();
    r += (radiusTarget - r)*growSpeed;
    setRadius(r);
}

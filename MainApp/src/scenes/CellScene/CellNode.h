//
//  Cell.h
//  MainApp
//
//  Created by Todd Vanderlin on 3/9/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "Particle.h"

class CellNode : public Particle {
  
public:
    
    bool bRemove;
    
    CellNode() {
        bRemove = false;
        radius  = 3;  
        drag    = ofRandom(0.97, 0.99);
    }
    void draw() {
        ofCircle(pos, 1);
    }
    
    static bool shouldRemove(const CellNode &c) {
        return c.bRemove;
    }
};

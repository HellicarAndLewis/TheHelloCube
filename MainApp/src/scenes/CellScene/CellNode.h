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
    
    CellNode() {
        radius = 3;  
        drag   = 0.98;//ofRandom(0.97, 0.99);
    }
    
};

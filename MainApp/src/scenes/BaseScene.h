//
//  BaseScenes.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "Utils.h"
#include "AppAssets.h"

class BaseScene {
    
private:
protected:
public:
    
    BaseScene();
    ~BaseScene();
    
    // ----------------------------------------------------
    string name;
    
    // ----------------------------------------------------
    virtual void setup() = 0;
    virtual void update() {};
    virtual void draw();
    
    
    
};
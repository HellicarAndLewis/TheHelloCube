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
#include "ofxGui.h"

class BaseScene {
    
private:
protected:
public:
    
    BaseScene();
    ~BaseScene();
    
    // ----------------------------------------------------
    string   name;
    ofxPanel gui;
    
    // ----------------------------------------------------
    virtual void setup() = 0;
    virtual void update() {};
    virtual void draw();

    // ----------------------------------------------------
    virtual void keyPressed(int key){};
    virtual void keyReleased(int key){}
    virtual void mouseMoved(int x, int y){};
    virtual void mouseDragged(int x, int y, int button){};
    virtual void mousePressed(int x, int y, int button){};
    virtual void mouseReleased(int x, int y, int button){};
    
    
};
//
//  DrawScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"
#include "ofxBox2d.h"
#include "Viner.h"

class DrawnScene : public BaseScene {
    
private:
protected:
public:
    
    ofxBox2d        box2d;
    vector <Viner>  vines;
    
    ofxFloatSlider  damping;
    ofxFloatSlider  frequency;
    
    void setup();
    void update();
    void draw(); 
    void keyPressed(int key);
	void handleCommands(TwitterCommand& cmd, Effects& fx);    
};
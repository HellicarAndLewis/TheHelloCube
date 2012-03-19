//
//  CellScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"
#include "CellNode.h"
#include "ofxVoronoi.h"
#include "Utils.h"

class CellScene : public BaseScene {
    
private:
protected:
public:
  
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void addCells();
	void handleCommands(TwitterCommand& cmd, Effects& fx);
    void respondToNewComplimentaryColours();
    void enterScene() {
        generateComplimentaryColours();
        respondToNewComplimentaryColours();
        ofRegisterMouseEvents(&gui);
    }
	
    ofxVoronoi        voronoi;
    vector <CellNode> cells;
    
    ofxFloatSlider separationDistance;
    ofxFloatSlider damping;
    ofxFloatSlider maxParticleSpeed;
    
    ofxFloatSlider releaseRate;
    ofxIntSlider   maxCellsOnScreen;
    bool drawGUI;
};
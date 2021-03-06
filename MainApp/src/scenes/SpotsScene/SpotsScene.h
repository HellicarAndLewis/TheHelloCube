//
//  SpotScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "BaseScene.h"
#include "ofxBox2d.h"
#include "SpotShape.h"
#include "ofxBox2dPolygonUtils.h"
#include "GeometryUtils.h"


class SpotsScene : public BaseScene {
    
private:
protected:
public:
    
    void setup();
    void update();
    void draw();
	virtual void drawGui();
    void keyPressed(int key);
    void addPoints();
    void makeObstacles();
    void addShape();
    void exitScene();
    
	void handleCommands(TwitterCommand& cmd, Effects& fx);
    float speed;
    ofxBox2d box2d;
    vector <ofxBox2dCircle> obsticals;
    vector <SpotShape>   shapes;
    vector <ofVec2f>         pts;
    vector <TriangleShape>   tris;
    
    bool     circleFrcFlip;
    ofVec2f circleFrc;
    ofxFloatSlider releaseRate;
    ofxIntSlider   maxShapesOnScreen;
    
};